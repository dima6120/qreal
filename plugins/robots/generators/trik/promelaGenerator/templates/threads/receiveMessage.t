@@RECEIVER@@chan?@@SENDER@@proc(temp);
d_step {
	int receive_i;
	for (receive_i : 0 .. temp.size - 1) {
		@@ASSIGNMENT@@;
	}
	@@VARIABLE@@.size = temp.size;
};