atomic {
	array temp;
	@@RECEIVER@@chan?@@SENDER@@proc(temp);
	copyStr(@@VARIABLE@@, temp.a[0].s);
}