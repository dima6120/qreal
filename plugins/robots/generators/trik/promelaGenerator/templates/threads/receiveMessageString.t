atomic {
	message temp;
	@@RECEIVER@@chan?@@SENDER@@proc(temp);
	copyStr(@@VARIABLE@@, @@SENDER@@proc_strings[0]);
};