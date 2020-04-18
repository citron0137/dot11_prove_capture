#include "common.h"
#include "capture_pkt.h"
#include "save_pkt.h"



queue<Packet> pkts;

void * handle_pkt(void * context){
	auto data = &((save_pkt *)context)->vdata;
	time_t start = time(NULL);
	while(1){
	
		//cout << string(100, '\n');
		//cout << "\033[0;0f";
		for(auto iter =  data->begin(); iter != data->end(); ++iter)
		{
			/*	
			cout << "mac : "<< iter->first 
				<< "----- \tstart : " << iter->second->stime 
				<< "\tend : " << iter->second->etime << endl;
			*/
			//cout << iter->second->to_string(iter->first) << endl;
		}
		//cout << "passed time : " << time(NULL) - start << " | size : " << data->size() << endl;
		sleep(1);
	}
	/*
	while(1){
		if(pkts.size() > 1 ){
			Packet pkt = pkts.front();
			if(pkt.pdu()->find_pdu<RadioTap>() && pkt.pdu()->find_pdu<Dot11ProbeRequest>()){
				cout << "At:" << pkt.timestamp().seconds() << " - " //<<endl;
				 << (int)pkt.pdu()->rfind_pdu<RadioTap>().dbm_signal() //<< endl;
				 << " - " << pkt.pdu()->rfind_pdu<Dot11ProbeRequest>().addr2() << endl;
			}
			pkts.pop();
		}
	}
	*/

	return 0;
}

int main()
{


	save_pkt saver(5);
	saver.sync_with_db();
	capture_pkt capturer("mon0");
	capturer.add_output_queue(&(saver.input_que));	
	
	pthread_t capture_thr;
	pthread_create(&capture_thr, NULL, &capture_pkt::do_capture_thr, &capturer);
	
	pthread_t save_thr;
	pthread_create(&save_thr, NULL, &save_pkt::do_save_thr, &saver);
	

	handle_pkt(&saver);
	pthread_join(capture_thr,NULL);
	pthread_join(save_thr,NULL);
	//handle_pkt(&saver);
	//pthread_t show_thr;
	//pthread_create(&show_thr, NULL, &handle_pkt, NULL);
	
	/*
	pthread_t show_thr;
	pthread_create(&show_thr, NULL, &handle_pkt, NULL);

	*/
}
