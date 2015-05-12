all: myfsck

test:
	python ~cs537-1/testing/p5b/FSCKTest.pyc .

img1:
	./myfsck ./images/image1.img > ./logs/output1.txt
	cp ./badimages/image1.img images/
	cat ./logs/output1.txt

img2:
	./myfsck ./images/image2.img > ./logs/output2.txt
	cp ./badimages/image2.img images/
	cat ./logs/output2.txt

img3:
	./myfsck ./images/image3.img > ./logs/output3.txt
	cp ./badimages/image3.img images/
	cat ./logs/output3.txt

img4:
	./myfsck ./images/image4.img > ./logs/output4.txt
	cp ./badimages/image4.img images/
	cat ./logs/output4.txt

wsb:
	./myfsck ./images/wrongsizesb.img > ./logs/wrongsizesb.txt
	rm -f ./images/wrongsizesb.img
	scp cook@best-linux.cs.wisc.edu:~cs537-1/testing/p5b/images/wrongsizesb.img ./images/.
	cat ./logs/wrongsizesb.txt

myfsck: fsck.c
	gcc fsck.c -o myfsck -Wall -Werror -m32

clean: 
	rm -rf myfsck
