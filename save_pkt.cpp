#include "common.h"
#include "save_pkt.h"
#include <jsoncpp/json/json.h>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

visit_data::visit_data(int stime, int etime, int st){
	this->stime = stime;
	this->etime = etime;
	this->st = st;
}

string visit_data::to_string(string hw_addr){
	return "{"\
	"\"hw_addr\":\""+hw_addr+"\","\
	"\"start_time\":"+std::to_string(stime)+","\
	"\"end_time\":"+std::to_string(etime)+","\
	"\"status\":"+std::to_string(st)+""\
	"}";
	
}
save_pkt::save_pkt(int syctime){
	this->syctime= syctime;
	this->pdtime = 10*60;

}

int save_pkt::sync_with_db(){
	cout << "sync with db" << endl;
	http_client client("http://dpc.radh.co.kr/");
	auto resp = client.request(
			"GET",
			"/capture.php"
			).get();
	string datas = resp.extract_string(true).get();
	//cout << datas << endl;	
	Json::Reader reader;
    Json::Value obj;	
	reader.parse(datas, obj);
	
	
	
	for(int i;i < obj.size(); i++ ){
		Json::StyledWriter styledWriter;
		string hw_addr = styledWriter.write(obj[i]["hw_addr"]).substr(1,17); 
		//cout << hw_addr << endl;
		//cout << "\t" << obj[i]["start_time"] << endl; 
		vdata.insert(pair<string,visit_data *>(
			hw_addr,
			new visit_data(
				obj[i]["start_time"].asInt(),
				obj[i]["end_time"].asInt(),
				obj[i]["status"].asInt()	
			)));
	}
	//cout << vdata.size() << endl;

	return 0;
}

int save_pkt::push(string body_data)
{
	http_client client("http://dpc.radh.co.kr/");
	auto resp = client.request(
			"POST",
			"/capture.php",
			body_data
			).get();
	/*	
	cout << "STATUS : " << resp.status_code() << endl;
	cout << "content-type : " << resp.headers().content_type() << endl;
	cout << resp.extract_string(true).get() << endl;
*/
	return 0;
}

void * save_pkt::do_save(){
	//	cout << "here i am" <<endl;
	while(1){
		// 10초마다 동기화
		unsigned int now_s = time(NULL);
		if(now_s % syctime == 0){
			for(auto iter =  vdata.begin(); iter != vdata.end();){
				try{
				push(iter->second->to_string(iter->first));
				}catch(...){
					continue;
				}
				iter->second->st = 1;
				if(iter->second->etime + pdtime < now_s){
					iter->second->~visit_data();
					vdata.erase(iter++);
				}
				else{
					iter++;
				}
			}
			sleep(1);
		}
		
				
		if(input_que.size() > 1 ){
			Packet pkt = input_que.front();
			if(pkt.pdu()->find_pdu<RadioTap>() && pkt.pdu()->find_pdu<Dot11ProbeRequest>()){
				

				string addr = pkt.pdu()->rfind_pdu<Dot11ProbeRequest>().addr2().to_string();
				auto time = pkt.timestamp().seconds();
				//handle
				auto iter = vdata.lower_bound(addr);
				auto uiter = vdata.upper_bound(addr);
				for (iter; iter != uiter; ++iter){
					if(iter->second->etime+pdtime > time){
						iter->second->etime = time;
						break;
					}
				}
				if( iter == uiter){
					vdata.insert(pair<string,visit_data *>(
						addr,	
						new visit_data(
							time,
							time,
							0
						)));
				}

			
				
				
				//
				//cout << "success -------------------!" << vdata.size()  << endl;
			}
			input_que.pop();
		}
	}
	return 0;
}

void * save_pkt::do_save_thr(void * context){
	return ((save_pkt *)context)->do_save();
}
