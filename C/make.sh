for i in `ls *.c` ; do a=`echo $i | cut -d '.' -f 1` ; gcc $a.c -o $a ; done

