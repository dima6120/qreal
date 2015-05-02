atomic {
	message temp;
	temp.size = @@SIZE@@;
@@MESSAGE@@
	@@RECEIVER@@chan!@@SENDER@@proc(temp);
};