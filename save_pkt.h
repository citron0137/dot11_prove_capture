
class visit_data{
	public:
	int stime;
	int etime;
	int st;

	visit_data(int stime, int etime, int st);
	string to_string(string hw_addr);
};

class save_pkt{
public:
	int pdtime;
	int syctime;
	queue<Packet> input_que;
	multimap<string,visit_data * > vdata;
	
	save_pkt(int pdtime);
	int sync_with_db();
	int push(string body_data);
	void * do_save();
	static void * do_save_thr(void * context);
};

