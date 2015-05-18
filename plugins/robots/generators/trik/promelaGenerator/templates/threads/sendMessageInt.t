d_step {
	temp.size = 1;
	temp.a[0] = @@MESSAGE@@;
};
@@RECEIVER@@chan!@@SENDER@@proc(temp);
