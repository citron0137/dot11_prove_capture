#include "./common.h"
#include "capture_pkt.h"


capture_pkt::capture_pkt(string inf){
	interface = inf;
	logpath = "./log/";
	config.set_promisc_mode(true);
	config.set_rfmon(true);
	writer = new PacketWriter(logpath+"test.pacp", DataLinkType<RadioTap>());	
}



void * capture_pkt::do_capture(){
	Sniffer sniffer(interface, config);
	while(1){
		Packet pkt = sniffer.next_packet();	
		writer->write(pkt);
		for(auto iter = output_queues.begin(); iter != output_queues.end(); ++iter ){
			(*iter)->push(pkt);
		}
	}
	return 0;
}

void * capture_pkt::do_capture_thr(void * context){
	return ((capture_pkt *)context)->do_capture();
}

bool capture_pkt::add_output_queue(queue<Packet> * que){
	
	output_queues.push_back(que);	
	return true;
}
