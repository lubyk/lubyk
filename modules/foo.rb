#!/bin/ruby
%w{bit crypto cv db dub dummy editor gl glut lfs lk mdns midi mimas qtbug.tgz remdebug sqlite3 test video wii xml yaml zmq}.each do |d|
  system("cd #{d} && git commit --amend -m 'Fix lib path'")
end
