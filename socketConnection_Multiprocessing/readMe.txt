This project contains simple socket server-client program, where server listens to multiple requests using multiprocessing.
Process split using fork,
  child process connects to current request
  parent process continues to listen to new request
