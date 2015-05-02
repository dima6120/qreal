atomic {
	message temp;
	@@RECEIVER@@chan?@@SENDER@@proc(temp);
	@@VARIABLE@@ = temp.a[0];
};