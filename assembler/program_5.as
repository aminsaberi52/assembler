	add   5,6,7		#0x00675000 // 6770688
labl	halt			#0x0e000000 // 234881024
a8	add   5,6,7		#0x00675000 // 6770688
labl	halt			#exit(1) errror-duplicate lable!!!
	add   5,6,7		#0x00675000 // 6770688
frs	slti	5,6,70		#0x06650046 // 107282502
scn2	ori	7,8,90		#0x0787005a // 126287962
	halt			#0x0e000000 // 234881024
ll	.fill	55		#0x00000037 // 55
l2	.fill	54		#0x00000036 // 54