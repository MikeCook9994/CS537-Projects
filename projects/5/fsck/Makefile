all: myfsck

scp cook@best-linux.cs.wisc.edu:~cs537-1/testing/p5b/images/.img

test:
	python ~cs537-1/testing/p5b/FSCKTest .

baddir:
	./myfsck ./images/baddirectory.img > ./logs/baddirectory.txt
	rm -f ./images/baddirectory.img
	cp ./corruptedimages/baddirectory.img ./images
	cat ./logs/baddirectory.txt

badlink:
	./myfsck ./images/badlinkcount.img > ./logs/badlinkcount.txt
	rm -f ./images/badlinkcount.img
	cp ./corruptedimages/badlinkcount.img ./images
	cat ./logs/badlink.txt

badsb:
	./myfsck ./images/badsuperblock.img > ./logs/badsuperblock.txt
	rm -f ./images/badsuperblock.img
	cp ./corruptedimages/badsuperblock.img ./images
	cat ./logs/badsuperblock.txt

badtype:
	./myfsck ./images/badtype.img > ./logs/badtype.txt
	rm -f ./images/badtype.img
	cp ./corruptedimages/badtype.img ./images
	cat ./logs/badtype.txt

noref:
	./myfsck ./images/noreferenceinode.img > ./logs/noref.txt
	rm -f ./images/noreferenceinode.img
	cp ./corruptedimages/noreferenceinode.img ./images
	cat ./logs/noref.txt

notfree:
	./myfsck ./images/blocknotfree.img > ./logs/notfree.txt
	rm -f ./images/blocknotfree.img
	cp ./corruptedimages/blocknotfree.img ./images
	cat ./logs/notfree.txt

wsb:
	./myfsck ./images/wrongsizesb.img > ./logs/wsb.txt
	rm -f ./images/wrongsizesb.img
	cp ./corruptedimages/wrongsizesb.img ./images
	cat ./logs/wsb.txt

myfsck: fsck.c
	gcc fsck.c -o myfsck -Wall -Werror -m32

clean: 
	rm -rf myfsck
