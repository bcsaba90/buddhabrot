for i in `ls ../*.bmp`; do convert "$i" "$i.png"; echo "Converted $i to $i.png"; done
