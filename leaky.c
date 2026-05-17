#include <stdio.h>

void main()
{
    int bucketcap,packets,leakrate,i;
    int leaked,bucket=0;
    printf("Enter the bucket capacity = ");
    scanf("%d",&bucketcap);
    printf("Enter leakrate = ");
    scanf("%d",&leakrate);
    printf("Enter total no.of packets = ");
    scanf("%d",&packets);
    int packet[packets];
    printf("Enter the packets:-\n");
    for(i=0;i<packets;i++)
    {
        scanf("%d",&packet[i]);
    }

    printf("Leaky bucket simulation.....\n");
    printf("Time\tpacket\tbucket\tleaked\tremaining\n");
    for(i=0;i<packets;i++)
    {
        printf("%d\t%d",i+1,packet[i]);
        bucket=bucket+packet[i];
        if(bucket>bucketcap)
        {
            printf("\t%d (overflow %d)",bucketcap,bucket-bucketcap);
            bucket=bucketcap;
        }
        else
        {
            printf("\t%d",bucket);
        }
        if(bucket>=leakrate)
            leaked=leakrate;
        else
            leaked=bucket;
        bucket=bucket-leaked;
        printf("\t%d\t%d\n",leaked,bucket);
    }
    int time=i+1;
    while(bucket>0)
    {
        if(bucket>=leakrate)
            leaked=leakrate;
        else
            leaked=bucket;
        printf("%d\t0\t%d\t%d\t%d\n",time,bucket,leaked,bucket-leaked);
        bucket=bucket-leaked;
        time++;
    }
}