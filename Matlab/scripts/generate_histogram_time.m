function generate_histogram_time()

files = dir('*.txt');

% Inicializa vari?veis
gst_norm = zeros(2);
gst_lapl = zeros(2);
gst_curv = zeros(2);
gst_doug = zeros(2);
gst_lapl_curv = zeros(2);
gst_lapl_doug = zeros(2);

gmt_norm = zeros(2);
gmt_lapl = zeros(2);
gmt_curv = zeros(2);
gmt_doug = zeros(2);
gmt_lapl_curv = zeros(2);
gmt_lapl_doug = zeros(2);

glt_norm = zeros(2);
glt_lapl = zeros(2);
glt_curv = zeros(2);
glt_doug = zeros(2);
glt_lapl_curv = zeros(2);
glt_lapl_doug = zeros(2);

%0-40, 40-80, 80-120
group = 40;

% 0 - Normal
% 1 - Laplacian
% 2 - Curvature
% 3 - DouglasPeucker
% 4 - Laplacian + Curvature
% 5 - Laplacian + DouglasPeucker

for cntfiles=1:length(files)
	fin = fopen(files(cntfiles).name);
    tokens_name_file = strsplit(files(cntfiles).name,'_');
    while ~feof(fin)
        line = fgetl(fin);
        tokens = strsplit(line,' ');
        if(g_short(tokens, group))
            %Gesto curto
            if(strcmp(tokens_name_file(4), '0'))
                gst_norm(1) = str2double(tokens(7));
                gst_norm(1) = gst_norm(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '1'))
                gst_lapl(1) = str2double(tokens(7));
                gst_lapl(1) = gst_lapl(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '2'))
                gst_curv(1) = str2double(tokens(7));
                gst_curv(1) = gst_curv(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '3'))
                gst_doug(1) = str2double(tokens(7));
                gst_doug(1) = gst_doug(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '4'))
                gst_lapl_curv(1) = str2double(tokens(7));
                gst_lapl_curv(1) = gst_lapl_curv(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '5'))
                gst_lapl_doug(1) = str2double(tokens(7));
                gst_lapl_doug(1) = gst_lapl_doug(1) + 1;
            end
        end
        if(g_median(tokens, group))
            %Gesto m?dio
            if(strcmp(tokens_name_file(4), '0'))
                gmt_norm(1) = str2double(tokens(7));
                gmt_norm(1) = gmt_norm(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '1'))
                gmt_lapl(1) = str2double(tokens(7));
                gmt_lapl(1) = gmt_lapl(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '2'))
                gmt_curv(1) = str2double(tokens(7));
                gmt_curv(1) = gmt_curv(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '3'))
                gmt_doug(1) = str2double(tokens(7));
                gmt_doug(1) = gmt_doug(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '4'))
                gmt_lapl_curv(1) = str2double(tokens(7));
                gmt_lapl_curv(1) = gmt_lapl_curv(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '5'))
                gmt_lapl_doug(1) = str2double(tokens(7));
                gmt_lapl_doug(1) = gmt_lapl_doug(1) + 1;
            end
        end
        if(g_long(tokens, group))
            %Gesto longo
            if(strcmp(tokens_name_file(4), '0'))
                glt_norm(1) = str2double(tokens(7));
                glt_norm(1) = glt_norm(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '1'))
                glt_lapl(1) = str2double(tokens(7));
                glt_lapl(1) = glt_lapl(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '2'))
                glt_curv(1) = str2double(tokens(7));
                glt_curv(1) = glt_curv(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '3'))
                glt_doug(1) = str2double(tokens(7));
                glt_doug(1) = glt_doug(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '4'))
                glt_lapl_curv(1) = str2double(tokens(7));
                glt_lapl_curv(1) = glt_lapl_curv(1) + 1;
            end
            if(strcmp(tokens_name_file(4), '5'))
                glt_lapl_doug(1) = str2double(tokens(7));
                glt_lapl_doug(1) = glt_lapl_doug(1) + 1;
            end
        end
    end
end

end

function r = g_short(tokens, group)
    mx = max(tokens);
    if(mx >= 0 && mx < group)
      r = 1;
    else
      r = 0;
    end
end

function r = g_median(tokens, group)
    mx = max(tokens);
    if(mx >= group && mx < 2 * group)
      r = 1;
    else
      r = 0;
    end
end

function r = g_long(tokens, group)
    mx = max(tokens);
    if(mx >= 2 * group && mx < 3 * group)
      r = 1;
    else
      r = 0;
    end
end

function m = max(tokens)
  left = str2double(tokens(3));
  right = str2double(tokens(6));
  if(left > right)
      m = left;
  else
      m = right;
  end
end
