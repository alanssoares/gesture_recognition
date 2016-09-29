function mR = custom_confusion(method, threshold)

mR = zeros(4,7);

files = dir('*.txt');

for cntfiles=1:length(files)
    
    fin = fopen(files(cntfiles).name);
    gestures = ['g1','g2','g3','g4','g5','g6','g7'];
    tokens_name_file = strsplit(files(cntfiles).name,'_');
    
    if(strcmp(tokens_name_file(3), method) && ...
      strcmp(tokens_name_file(4), threshold))
      while ~feof(fin)
        line = fgetl(fin);
        tokens_line = strsplit(line,' ');
        if(is_vp(line))
		c = findPosition(tokens_line(1), gestures);
		mR(1,c) = mR(1,c) + 1;
        end
        if(is_vn(line))
		c = findPosition(tokens_line(1), gestures);
		mR(2,c) = mR(2,c) + 1;
        end
        if(is_fp(line))
		c = findPosition(tokens_line(1), gestures);
		mR(3,c) = mR(3,c) + 1;
        end
        if(is_fn(line))
		c = findPosition(tokens_line(1), gestures);
		mR(3,c) = mR(3,c) + 1;
        end
      end
    end
  fclose(fin);
end
end

% Encontra o número do gesto
function col = findPosition(g1, gestures)
	col = 0;
	for g2 = gestures
		if(strcmp(g1, g2) == 1)
			col = str2double(g1(2));
		end
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
