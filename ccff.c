/* ccff.c RV */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/timeb.h>

#define BUF (1024*1024)
#define MB 1048576

void usage(char *);


void usage(char *pname)
{
         printf("%s -w filename  #MB\n",pname);
         printf("%s -r filename\n",pname);
}


main(int argc, char * argv[])
{
	long j,i;
	int of,inf;
	char *obuf;
	char *ibuf;
	struct timeb before,after;
	float mbw,mbr;
	char *rw=argv[1];
	char *fname=argv[2];
	char *siz=argv[3];

	if (argc < 3) {
		usage(argv[0]);
		return -1;
	}
		
	if ( rw[1] != 'w' &&  rw[1] != 'r' ) {
		usage(argv[0]);	
		return -1;
	}

	if (rw[1] == 'w') {
	 	unsigned long mb;	
	
		obuf = malloc(BUF);
		memset(obuf,0,BUF);
	
		of = open(fname, O_CREAT | O_WRONLY );
		mb=atol(siz);
		
		ftime(&before);
		
		for (i=0 ; i<((MB/BUF)*mb) ; i++)
			write(of,obuf,BUF);
		
		ftime(&after);
		
		long ttw=after.time-before.time;
		
		if (ttw > 0)
			mbw = ((MB*mb)/(float)MB)/ttw;
		
		printf("%f MB/s\n",mbw);

		close(of);
		free(obuf);
	}

	if (rw[1] == 'r') {

		unsigned long byt = 0;		
		ssize_t r;

		ibuf = malloc(BUF);
		memset(ibuf,0,BUF);

		inf = open(fname, O_RDONLY );

		ftime(&before);

		while((r=read(inf,ibuf,BUF)) > 0) {
			byt+=r;
		}
		
                ftime(&after);

		long ttr=after.time-before.time;

		if (ttr > 0)
			mbr = (byt/(float)MB)/ttr;

		printf("%f MB/s\n",mbr);

		close(inf);
		free(ibuf);
	}
			
	return 0;
}
