function mR = gesture_matrix_confusion(method, threshold, partfile)

mR = zeros(7,7);
before = 'g1';
i = 1;

files = dir(partfile);

for cntfiles=1:length(files)

    fin = fopen(files(cntfiles).name);

    tokens_name_file = strsplit(files(cntfiles).name,'_');

    if(strcmp(tokens_name_file(4), method) && ...
      strcmp(tokens_name_file(5), threshold))
      while ~feof(fin)

        line = fgetl(fin);

        tokens_line = strsplit(line,' ');

        if(strcmp(tokens_line(1), before) == 0)
            before = tokens_line(1);
            i = i + 1;
        end

        if(strcmp(tokens_line(4), 'g1'))
            mR(i,1) = mR(i,1) + 1;
        end
        if(strcmp(tokens_line(4), 'g2'))
            mR(i,2) = mR(i,2) + 1;
        end
        if(strcmp(tokens_line(4), 'g3'))
            mR(i,3) = mR(i,3) + 1;
        end
        if(strcmp(tokens_line(4), 'g4'))
            mR(i,4) = mR(i,4) + 1;
        end
        if(strcmp(tokens_line(4), 'g5'))
            mR(i,5) = mR(i,5) + 1;
        end
        if(strcmp(tokens_line(4), 'g6'))
            mR(i,6) = mR(i,6) + 1;
        end
        if(strcmp(tokens_line(4), 'g7'))
            mR(i,7) = mR(i,7) + 1;
        end
      end
    end
  fclose(fin);
end
end
