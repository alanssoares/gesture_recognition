function extract_hands_utkinect_action()

fileID = fopen('ut_kinect_action.txt','w');
fileLabel = load('actionLabel.txt','-ascii');

% Para cada label, abra o arquivo
% percorra at? encontrar o frame inicial
% finalize quando encontrar o frame final
n = length(fileLabel(:,1));
i = 0;
f = 1;
while (i < n)
    
    name = getFileName(fileLabel(i + 1, 1));
    file_gesture = load(name, '-ascii');
    
    i = i + 1;
    k = i;
    t = 10;
    while t > 0
        
        frame_ini = fileLabel(k + 1, 2);
        frame_fin = fileLabel(k + 1, 3);
        
        j = 1;
        while (frame_ini < file_gesture(j,1))
            j = j + 1;
        end

        % In?cio de um gesto
        fprintf(fileID,'gesture %s hands 2\n', num2str(fileLabel(k + 1, 1)));
        while (frame_fin >= file_gesture(j,1))
            % Pego as posi??es da m?o e salvo
            fprintf(fileID,'%f %f %f %f %f %f\n', file_gesture(j, 22), ...
                 file_gesture(j, 23), ...
                 file_gesture(j, 24), ...
                 file_gesture(j, 34), ...
                 file_gesture(j, 35), ...
                 file_gesture(j, 36));
             j = j + 1;
        end
        fprintf(fileID, 'end\n');
        k = k + 1;
        t = t - 1;
    end
    f = f + 1;
    i = i + 10;
end

function name = getFileName(name)
    name = strcat('joints_', num2str(name));
    name = strcat(name, '.txt');
end

end