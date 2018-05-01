afile = File.new('../../../log.txt', 'r')
fsize = afile.stat.size

content = afile.sysread(fsize)
puts content