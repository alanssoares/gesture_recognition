function mR = custom_confusion(method, threshold)

mR = zeros(4,7);

files = dir('*.txt');

for cntfiles=1:length(files)
    
    fin = fopen(files(cntfiles).name);
    
    tokens_name_file = strsplit(files(cntfiles).name,'_');
    
    if(strcmp(tokens_name_file(3), method) && ...
      strcmp(tokens_name_file(4), threshold))
      while ~feof(fin)
        line = fgetl(fin);
        tokens_line = strsplit(line,' ');
        if(is_vp(line))
            if(strcmp(tokens_line(1), 'g1'))
                mR(1,1) = mR(1,1) + 1;
            end
            if(strcmp(tokens_line(1), 'g2'))
                mR(1,2) = mR(1,2) + 1;
            end
            if(strcmp(tokens_line(1), 'g3'))
                mR(1,3) = mR(1,3) + 1;
            end
            if(strcmp(tokens_line(1), 'g4'))
                mR(1,4) = mR(1,4) + 1;
            end
            if(strcmp(tokens_line(1), 'g5'))
                mR(1,5) = mR(1,5) + 1;
            end
            if(strcmp(tokens_line(1), 'g6'))
                mR(1,6) = mR(1,6) + 1;
            end
            if(strcmp(tokens_line(1), 'g7'))
                mR(1,7) = mR(1,7) + 1;
            end
        end
        if(is_vn(line))
            if(strcmp(tokens_line(1), 'g1'))
                mR(2,1) = mR(2,1) + 1;
            end
            if(strcmp(tokens_line(1), 'g2'))
                mR(2,2) = mR(2,2) + 1;
            end
            if(strcmp(tokens_line(1), 'g3'))
                mR(2,3) = mR(2,3) + 1;
            end
            if(strcmp(tokens_line(1), 'g4'))
                mR(2,4) = mR(2,4) + 1;
            end
            if(strcmp(tokens_line(1), 'g5'))
                mR(2,5) = mR(2,5) + 1;
            end
            if(strcmp(tokens_line(1), 'g6'))
                mR(2,6) = mR(2,6) + 1;
            end
            if(strcmp(tokens_line(1), 'g7'))
                mR(2,7) = mR(2,7) + 1;
            end
        end
        if(is_fp(line))
            if(strcmp(tokens_line(1), 'g1'))
                mR(3,1) = mR(3,1) + 1;
            end
            if(strcmp(tokens_line(1), 'g2'))
                mR(3,2) = mR(3,2) + 1;
            end
            if(strcmp(tokens_line(1), 'g3'))
                mR(3,3) = mR(3,3) + 1;
            end
            if(strcmp(tokens_line(1), 'g4'))
                mR(3,4) = mR(3,4) + 1;
            end
            if(strcmp(tokens_line(1), 'g5'))
                mR(3,5) = mR(3,5) + 1;
            end
            if(strcmp(tokens_line(1), 'g6'))
                mR(3,6) = mR(3,6) + 1;
            end
            if(strcmp(tokens_line(1), 'g7'))
                mR(3,7) = mR(3,7) + 1;
            end
        end
        if(is_fn(line))
            if(strcmp(tokens_line(1), 'g1'))
                mR(4,1) = mR(4,1) + 1;
            end
            if(strcmp(tokens_line(1), 'g2'))
                mR(4,2) = mR(4,2) + 1;
            end
            if(strcmp(tokens_line(1), 'g3'))
                mR(4,3) = mR(4,3) + 1;
            end
            if(strcmp(tokens_line(1), 'g4'))
                mR(4,4) = mR(4,4) + 1;
            end
            if(strcmp(tokens_line(1), 'g5'))
                mR(4,5) = mR(4,5) + 1;
            end
            if(strcmp(tokens_line(1), 'g6'))
                mR(4,6) = mR(4,6) + 1;
            end
            if(strcmp(tokens_line(1), 'g7'))
                mR(4,7) = mR(4,7) + 1;
            end
        end
      end
    end
  fclose(fin);
end
end

% Vefifica se eh um verdadeiro positivo
function result = is_vp(line)
    result = 0;
    tokens = strsplit(line,' ');
    if(strcmp(tokens(1), tokens(4)) == 1 && ... 
        strcmp(tokens(9), '1'))
        result = 1;
    end
end

% Vefifica se eh um verdadeiro negativo
function result = is_vn(line)
    result = 0;
    tokens = strsplit(line,' ');
    if(strcmp(tokens(1), tokens(4)) == 0 && ... 
        strcmp(tokens(9), '1'))
        result = 1;
    end
end

% Vefifica se eh um falso positivo
function result = is_fp(line)
    result = 0;
    tokens = strsplit(line,' ');
    if(strcmp(tokens(1), tokens(4)) == 0 && ... 
        strcmp(tokens(9), '0'))
        result = 1;
    end
end

% Vefifica se eh um falso negativo
function result = is_fn(line)
	result = 0;
    tokens = strsplit(line,' ');
    if(strcmp(tokens(1), tokens(4)) == 1 && ... 
        strcmp(tokens(9), '0'))
        result = 1;
    end
end