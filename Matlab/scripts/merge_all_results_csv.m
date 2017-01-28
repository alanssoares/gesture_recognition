function merge_all_results_csv()

files = dir('*.txt');

fileID0 =fopen('csv_results_cross_validation_0.csv','wt');
fileID1 =fopen('csv_results_cross_validation_1.csv','wt');
fileID2 =fopen('csv_results_cross_validation_2.csv','wt');
fileID3 =fopen('csv_results_cross_validation_3.csv','wt');
fileID4 =fopen('csv_results_cross_validation_4.csv','wt');
fileID5 =fopen('csv_results_cross_validation_5.csv','wt');

for cntfiles=1:length(files)
	fin = fopen(files(cntfiles).name);
    name_tokens = strsplit(files(cntfiles).name, '_');
	while ~feof(fin)
        if(strcmp(name_tokens(4), '0'))
            fprintf(fileID0,'%s', strrep(fgetl(fin), ' ', ';'));
            fprintf(fileID0,';%s\n', strrep(files(cntfiles).name, '_', ';'));
        end
        if(strcmp(name_tokens(4), '1'))
            fprintf(fileID1,'%s', strrep(fgetl(fin), ' ', ';'));
            fprintf(fileID1,';%s\n', strrep(files(cntfiles).name, '_', ';'));
        end
        if(strcmp(name_tokens(4), '2'))
            fprintf(fileID2,'%s', strrep(fgetl(fin), ' ', ';'));
            fprintf(fileID2,';%s\n', strrep(files(cntfiles).name, '_', ';'));
        end
        if(strcmp(name_tokens(4), '3'))
            fprintf(fileID3,'%s', strrep(fgetl(fin), ' ', ';'));
            fprintf(fileID3,';%s\n', strrep(files(cntfiles).name, '_', ';'));
        end
        if(strcmp(name_tokens(4), '4'))
            fprintf(fileID4,'%s', strrep(fgetl(fin), ' ', ';'));
            fprintf(fileID4,';%s\n', strrep(files(cntfiles).name, '_', ';'));
        end
        if(strcmp(name_tokens(4), '5'))
            fprintf(fileID5,'%s', strrep(fgetl(fin), ' ', ';'));
            fprintf(fileID5,';%s\n', strrep(files(cntfiles).name, '_', ';'));
        end
	end
	fclose(fin);
end

fclose(fileID0);
fclose(fileID1);
fclose(fileID2);
fclose(fileID3);
fclose(fileID4);
fclose(fileID5);

end