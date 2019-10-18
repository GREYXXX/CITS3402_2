#jobarray is each part of adjacency matrix in each part of processor;
#temparrayA is k-th column we broadcast;
#temparrayB is k-th row we broadcast;

for(i = 0;i < n/p;i++){
	for(j = 0; j < n;j++){
		if(jobarray[i][j] > temparrayB[i] + tempA[j]){
			jobarray[i][j] = temparrayB[i] + temparrayA[j];
			path[i][j] = temppath[i];
		}
	}
}

for(i = 0;i < n/math.sqrt(p);i++){
        for(j = 0; j < n/math.sqrt(p);j++){
                if(jobarray[i][j] > temparrayB[i] + tempA[j]){
                        jobarray[i][j] = temparrayB[i] + temparrayA[j];
                        path[i][j] = temppath[i];
                }
        }
}
