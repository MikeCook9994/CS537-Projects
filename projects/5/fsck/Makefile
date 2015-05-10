all: myfsck

test:
	python ~cs537-1/testing/p5b/FSCKTest.pyc . > ./logs/testoutput.txt

img1:
	myfsck ./testImages/image1.img > ./logs/output1.txt
	cp ./corruptImages/image1.img testImages/

img2:
	myfsck ./testImages/image2.img > ./logs/output2.txt
	cp ./corruptImages/image2.img testImages/

img3:
	myfsck ./testImages/image3.img > ./logs/output3.txt
	cp ./corruptImages/image3.img testImages/

img4:
	myfsck ./testImages/image4.img > ./logs/output4.txt
	cp ./corruptImages/image4.img testImages/

myfsck: fsck.c
	gcc fsck.c -o myfsck -Wall -Werror -m32

clean: 
	rm -rf myfsck
