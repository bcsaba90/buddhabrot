# copy all animation files to current folder
j=0;for i in `ls -latr *.bmp | awk '{print $9}'`; do mv "$i" "frame_$j.bmp"; let j=$j+1; done
ffmpeg -y -i frame_%d.bmp.png -r 20 -vb 20M -vcodec mpeg4 -r 20 output.mp4
