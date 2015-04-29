atomic {
	array temp;
	@@RECEIVER@@chan?@@SENDER@@proc(temp);
	@@VARIABLE@@ = temp.a[0].i;
}