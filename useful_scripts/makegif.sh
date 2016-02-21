# copy all animation files to current folder
j=0;for i in `ls -latr *.bmp | awk '{print $9}'`; do mv "$i" "frame_$j.bmp"; let j=$j+1; done
ffmpeg -y -i frame_%d.bmp -vf fps=10,scale=320:-1:flags=lanczos,palettegen palette.png
ffmpeg -y -i frame_%d.bmp -i palette.png -r 24 -filter_complex "fps=24,scale=1920:-1:flags=lanczos[x];[x][1:v]paletteuse" output.gif
