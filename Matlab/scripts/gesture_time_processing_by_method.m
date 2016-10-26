function mR = gesture_time_processing_by_method(gesture, partfile)

mR = zeros(6,2);
i = 1;
j = 0;

files = dir(partfile);

for cntfiles=1:length(files)

    fin = fopen(files(cntfiles).name);

    while ~feof(fin)
        line = fgetl(fin);
        tokens_line = strsplit(line,' ');
        if(strcmp(tokens_line(1), gesture) == 1)
            mR(i,1) = mR(i,1) + str2double(tokens_line(3));% qtd points
            mR(i,2) = mR(i,2) + str2double(tokens_line(7));% time
            j = j + 1;
        end
    end

    mR(i,1) = mR(i,1) / j;
    mR(i,2) = mR(i,2) / j;
    i = i + 1;
    j = 0;
    fclose(fin);
end
end
