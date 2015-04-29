atomic {
	array temp;
	temp.a[0].size = 1;
	temp.a[0].i = @@MESSAGE@@;
	@@RECEIVER@@chan!@@SENDER@@proc(temp);
}