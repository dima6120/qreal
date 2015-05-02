atomic {
	message temp;
	@@RECEIVER@@chan?@@SENDER@@proc(temp);
	int receive_i;
	for (receive_i : 0 .. temp.size - 1) {
		@@ASSIGNMENT@@;
	}
	@@VARIABLE@@.size = temp.size;
};