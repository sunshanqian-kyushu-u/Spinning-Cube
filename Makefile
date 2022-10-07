all:
	arm-linux-gnueabihf-gcc main.c spin.c -o spincubeAPP -lm

clean:
	rm spincubeAPP

copy:
	sudo cp spincubeAPP /home/ubuntu20/workdir/nfsdir/rootfs/home/greenqueen/ -f