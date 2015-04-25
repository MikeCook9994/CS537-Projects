#include "types.h"
#include "stat.h"
#include "user.h"

void helloWorld() {
	printf(1, "Hello World\n");
}

int main(int argc, char * argv[]) {
	thread_create(helloWorld, NULL);
	exit();
}