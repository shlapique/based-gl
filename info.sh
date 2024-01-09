#!/bin/bash

### vars ###
num_vert=0
num_faces=0
num_triang=0
file=$1
while read line; 
do 
    mode=${line%% *}
    case $mode in
        v)
            ((num_vert=num_vert+1))
            ;;
        f)
            ((num_faces=num_faces+1))
            if [ "$(echo "$line" | wc -w)" == 3 ];
            then
                ((num_triang=num_triang+1))
            fi
            ;;
    esac
done < <(cat $file)
echo "polygons (faces) : $num_faces"
echo "vertices         : $num_vert"
echo "triangles        : $num_triang"
