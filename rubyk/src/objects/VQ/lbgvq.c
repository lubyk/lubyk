/* =====================================================================

LBG Vector Quantizer Design Program.

  by Nam Phamdo 				April 23, 2000.

  Copyright 2000, Nam Phamdo
  ===================================================================== */

#include <stdio.h>
#include <math.h>
  main(argc,argv)
{

  char    in_name[50];
  char    res_name[50];
  char    out_name[50];

  float	header[64];
  float   **data;
  float	**codebk;
  float   rvq, dist;

  int     *bin;
  int     no_tr_vec, dim;
  int     bpv; /* bpv = bits per vector of VQ*/
  int	cb_size;
  int     i, j, k, l, m, ll, ii, kk, nn;
  int     fd_in, fd_out;
  int     fsize, ssize, n_read, n_write;

  float   **yy;
  float   totd1, totd2, dist1, dist2;
  float   del, drel;
  int     flag1, index;
  int     mtemp;   /* = number of current vectors */


  FILE    *res_file, *fopen();

  fsize=4;
  ssize=2;

  printf (" Enter input data file: ");
  scanf ("%49s", in_name);

  printf (" Enter number of training vectors: ");
  scanf ("%d", &no_tr_vec);

  printf (" Enter dimension: ");
  scanf ("%d", &dim);

  printf (" Enter rate of LBG in bits/sample: ");
  scanf ("%f", &rvq);

  printf (" Enter filename of codebook: ");
  scanf ("%49s", out_name);

  printf (" Enter filename of results: ");
  scanf ("%49s", res_name);
  res_file  = fopen(res_name,"w");

  fprintf(res_file," \n");
  fprintf(res_file," \n");
  fprintf(res_file," Input data file            : %s\n",in_name);
  fprintf(res_file," Number of training vectors : %d\n",no_tr_vec);
  fprintf(res_file," Dimension                  : %d\n",dim);
  fprintf(res_file," Rate of VQ (bit/sample)    : %f\n",rvq);
  fprintf(res_file," Filename of VQ codebook    : %s\n",out_name);
  fprintf(res_file," Filename of results        : %s\n",res_name);
  fprintf(res_file," \n");
  fprintf(res_file," \n");

  bpv=rvq*dim+0.0001;
  cb_size = 1;
  for(i=1;i<=bpv;++i)
    cb_size=cb_size*2;

/* *********************ALLOCATING MEMORY****************** */
  if((data = (float **) malloc(no_tr_vec * sizeof(float*)))==NULL){
    fprintf(stderr,"no more memory\n");
    exit(1);
  }
  for(i=0;i<no_tr_vec;i++)
  if((data[i] = (float *) malloc((dim) * sizeof(float)))==NULL){
    fprintf(stderr,"no more memory\n");
    exit(1);
  }

  if((bin = (int *) malloc((cb_size) * sizeof(int)))==NULL){
    fprintf(stderr,"no more memory\n");
    exit(1);
  }

  if((codebk = (float **) malloc(cb_size * sizeof(float*)))==NULL){
    fprintf(stderr,"no more memory\n");
    exit(1);
  }
  for(i=0;i<cb_size;i++)
  if((codebk[i] = (float *) malloc((dim) * sizeof(float)))==NULL){
    fprintf(stderr,"no more memory\n");
    exit(1);
  }
  if((yy = (float **) malloc(cb_size * sizeof(float*)))==NULL){
    fprintf(stderr,"no more memory\n");
    exit(1);
  }
  for(i=0;i<cb_size;i++)
  if((yy[i] = (float *) malloc((dim) * sizeof(float)))==NULL){
    fprintf(stderr,"no more memory\n");
    exit(1);
  }
/* ******************************************************** */


/* ***************Reading in data*************** */
  fd_in = open(in_name,0);
  read(fd_in,header,256);
  for (i=0; i<no_tr_vec; ++i){
    if(read(fd_in,data[i],fsize*dim) != fsize*dim){
      printf(" Error reading data[%d]\n",i);
      fprintf(res_file," Error reading data[%d]\n",i);
      exit(0);
    }
  }
  n_read=close(fd_in);

/* *************Design VQ*********************************** */
  nn=no_tr_vec;
  if(nn<=cb_size){
    fprintf(stderr,"Training Size Too Small !!!\n");
    exit(1);
  }

  mtemp=1;
  for (k=0;k<dim;++k) {
    codebk[0][k]=0.0;
    for (i=0;i<nn;++i) {
      codebk[0][k]+=data[i][k];
    }
    codebk[0][k]/=(float) nn;
  }

  while (mtemp < cb_size) {

    mtemp=mtemp*2;
    for(i=0;i<mtemp;++i) {  /* splitting */
      j=i/2;
    del = 0.001;
    if(i == i/2*2) del=-0.001;
    for (k=0;k<dim;++k)
      yy[i][k]=codebk[j][k]*(1.0+del);
  }

  totd2=10.0e15;

  flag1 = 0;
  while ( flag1 == 0) {
    totd1=0.0;
    for(i=0;i<mtemp;++i) {
      for(k=0;k<dim;++k)
        codebk[i][k]=0.0;
      bin[i]=0;
    }
    for(i=0;i<nn;++i){
      j=0;
      dist2=0.0;
      for(k=0;k<dim;++k) {
        dist2+=(data[i][k]-yy[j][k])*(data[i][k]-yy[j][k]);
      }
      index=0;
      for(j=1;j<mtemp;++j){
        dist1=0.0;
        for(k=0;k<dim;++k)
          dist1+=(data[i][k]-yy[j][k])*(data[i][k]-yy[j][k]);
        if(dist1 < dist2){
          dist2=dist1;
          index=j;
        }
      }
      ++bin[index];
      for(k=0;k<dim;++k)
        codebk[index][k]+=data[i][k];
      totd1=totd1+dist2;
    }  /* end for i<nn */

    for(j=0;j<mtemp;++j){
      if(bin[j] > 0) {
        for(k=0;k<dim;++k){
          codebk[j][k]/=(float) bin[j];
          yy[j][k]=codebk[j][k];
        }
      }
    }

    totd1/=(float) nn*dim;
    drel=(totd2-totd1)/totd1;
    fprintf(stderr," cb_size=%4d  Ave. Dist = %f SNR = %f drel = %f \n",mtemp,totd1,-10.0*log10((double)totd1),drel);
    fprintf(res_file," cb_size=%4d  Ave. Dist = %f SNR = %f drel = %f \n",mtemp,totd1,-10.0*log10((double)totd1),drel);
    k=fflush(res_file);
    flag1 = 1;
    if(drel > 0.001) {
      flag1 = 0;
      totd2=totd1;
    }
  }   /* end while flag1 */

}   /* end while mtemp<cb_size */

dist=totd1;







/* *************Design VQ*********************************** */
fprintf(stderr,"\n");
fprintf(res_file,"\n");
fprintf(stderr," Ave. Distortion of VQ = %f\n",dist);
fprintf(stderr,"      SNR        of VQ = %f\n",-10.0*log10((double)dist));
fprintf(res_file," Ave. Distortion of VQ = %f\n",dist);
fprintf(res_file,"      SNR        of VQ = %f\n",-10.0*log10((double)dist));


fd_out = creat(out_name,0644);

for(i=0;i<cb_size;++i){
  if(write(fd_out,codebk[i],fsize*dim)!=fsize*dim){
    printf(" Error reading data[%d]\n",i);
    fprintf(res_file," Error reading data[%d]\n",i);
    exit(0);
  }
}
}



