function merge_samples()

files=dir('*.txt');
fileout='all_gestures.txt';
fout=fopen(fileout,'w');
for cntfiles=1:length(files)
	fin=fopen(files(cntfiles).name);
	while ~feof(fin)
		fprintf(fout,'%s\n',fgetl(fin));
	end
	fclose(fin);
end
fclose(fout);

end