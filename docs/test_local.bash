#start web service at http://localhost:8000
#root in current dir

#python3
uname="$(uname -s)"

port=8000

if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under GNU/Linux platform
    python3 -m http.server ${port}
elif [ "$(expr substr $(uname -s) 1 7)" == "MSYS_NT" ]; then
    # Do something under 64 bits Windows NT platform
    start python3 -m http.server ${port}
fi

#python2
#python -m SimpleHTTPServer
