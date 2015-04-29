atomic {
	array temp;
	temp.a[0].size = 1;
	copyStr(temp.a[0].s, @@MESSAGE@@); 
	@@RECEIVER@@chan!@@SENDER@@proc(temp);
}