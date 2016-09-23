function mR = gesture_matrix_confusion(method, threshold)

mR = zeros(7,7);
before = 'g1';
i = 1;

files = dir('*.txt');

for cntfiles=1:length(files)
    
    fin = fopen(files(cntfiles).name);
    
    tokens_name_file = strsplit(files(cntfiles).name,'_');
    
    if(strcmp(tokens_name_file(3), method) && ...
      strcmp(tokens_name_file(4), threshold))
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