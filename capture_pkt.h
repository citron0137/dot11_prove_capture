
class capture_pkt{
public:
	string logpath;
	PacketWriter * writer;
	string interface;
	SnifferConfiguration config;
	list<queue<Packet> *> output_queues;
	
	capture_pkt();
	capture_pkt(string interface);
	static void * do_capture_thr(void * context);
	void * do_capture();
	bool add_output_queue(queue<Packet> *que);
};
