DIR="external"

if [ -d "$DIR/rapidcsv" ]; then
   echo "$DIR rapidcsv exists!"
  else
  echo "$DIR Downloading rapidcsv:"
  git clone https://github.com/d99kris/rapidcsv.git "$DIR/rapidcsv"
fi

