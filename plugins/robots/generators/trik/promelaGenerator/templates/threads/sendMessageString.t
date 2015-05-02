atomic {
	message temp;
	temp.size = 1;
	copyStr(@@SENDER@@proc_strings[0], @@MESSAGE@@); 
	@@RECEIVER@@chan!@@SENDER@@proc(temp);
};