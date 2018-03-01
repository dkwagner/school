/* Handle the y[i] in x[] situation */
else if(array == 1){  /* We are handling a y array index */
  sprintf(buf, "      $$$ MPROC(%d): handling y[%d]\n", (int)pid, index);
  write(1, buf, strlen(buf));

  if(shmPtrY[index] < shmPtrX[0]){
    sprintf(buf, "      $$$ MPROC(%d): x[%d] = %d is found to be smaller than y[0] = %d\n",
      (int)pid, index, shmPtrY[index], shmPtrX[0]);
    write(1, buf, strlen(buf));
    shmPtrM[0] = shmPtrY[index];
  }
  else if(shmPtrY[index] > shmPtrX[xSize-1]){
    sprintf(buf, "      $$$ MPROC(%d): x[%d] = %d is found to be larger than y[%d] = %d\n",
      (int)pid, index, shmPtrY[index], xSize - 1, shmPtrX[xSize - 1]);
    write(1, buf, strlen(buf));
    shmPtrM[mSize -1] = shmPtrY[index];
  }
  else{
    left = 0;
    right = xSize - 1;
    while(1){
      mid = floor((left + right) / 2);
      if(shmPtrX[mid] > shmPtrY[index] && shmPtrX[mid - 1] < shmPtrY[index]){
        sprintf(buf, "      $$$ MPROC(%d): y[%d] = %d is found between x[%d] = %d and x[%d] = %d\n",
          (int)pid, index, shmPtrY[index], mid, shmPtrX[mid], mid - 1, shmPtrX[mid - 1]);
        write(1, buf, strlen(buf));
        sprintf(buf, "      $$$ MPROC(%d): about to write x[%d] = %d into position %d of the output array\n",
          (int)pid, index, shmPtrY[index], index + mid);
        write(1, buf, strlen(buf));
        shmPtrM[index + mid] = shmPtrY[index];
        exit(0);
      }
    }
  }
}

for(i = 0; i < ySize; i ++){
  if((fork_pid = fork()) == 0){  /* We are the child process */
    merge(i, 1, xSize, ySize, shmPtrX, shmPtrY, shmPtrM);
    exit(0);
  }
  else{  /* We are the parent process */
    continue;
  }
}
