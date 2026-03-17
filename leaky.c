#include <stdio.h>
#include <stdlib.h>

void main()
{
    int bucketcap,leakrate,packets,i;
    int leaked;
    int bucket=0;
    printf("Enter bucket capacity: ");
    scanf("%d",&bucketcap);
    printf("Enter leak rate: ");
    scanf("%d",&leakrate);
    printf("Enter no.of packets: ");
    scanf("%d",&packets);
    int packet[packets];
    printf("Enter incoming packets: ");
    for(i=0;i<packets;i++)
    {
        scanf("%d",&packet[i]);
    }
    printf("Leaky bucket simulation........\n");
    printf("Time\tpacket\tBucket\tleaked\tremaining\n");
    for(i=0;i<packets;i++)
    {
        printf("%d\t%d",i+1,packet[i]);
        bucket+=packet[i];
        if(bucket>=bucketcap)
        {
            printf("\t%d(overflow,dropped %d)",bucketcap,bucket-bucketcap);
            bucket=bucketcap;
        }
        else
        {
            printf("\t%d",bucket);
        }
        
        if(bucket>=leakrate)
        {
            leaked=leakrate;
        }
        else
        {
            leaked=bucket;
        }
        bucket-=leaked;
        printf("\t%d\t%d\n",leaked,bucket);
    }

    int time=packets+1;
    while(bucket>0)
    {
        if(bucket>=leakrate)
        {
            leaked=leakrate;
        }
        else
        {
            leaked=bucket;
        }
        printf("%d\t0\t%d\t%d\t%d\n",time,bucket,leaked,bucket-leaked);
        bucket-=leaked;
        time++;        
    }
}