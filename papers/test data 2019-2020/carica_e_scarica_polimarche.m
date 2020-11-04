%{ 
  dati sulla tensione delle celle con pacco batteria 16s1p acquisiti con ltc6813
  per maggiori informazioni fare riferimento a "risultati_carica_scaricaV3.pdf"

  file.mat in questa cartella sono state eliminate le colonne che non
  servivano e il tempo è stato convertito in secondi
  scaricaxxx.Tempo=scaricaxxx.Tempo/1000         %conversione millis->secondi

  per visualizzare ad una ad una tutti i plot utilizzare la funzione run and advance
%}
clc,clear,close all
load dati_carica_e_scarica_polimarche.mat

%% carica20/06/19 prima prova
plot_tensioni(carica20_06_19_prima_prova,'carica20/06/19 prima prova')

%% carica 20/06/19 2A seconda prova
plot_tensioni(carica20_06_19_2A_seconda_prova,'carica 20/06/19 2A seconda prova')

%% carica 20/06/19 2A terza prova
plot_tensioni(carica20_06_19_2A_terza_prova,'carica 20/06/19 2A terza prova')

%% scarica 1.6A 21/06/19
plot_tensioni(scarica21_06_19,'scarica 1.6A 21/06/19')

%% carica 21/06/19
plot_tensioni(carica21_06_19,'carica 21/06/19')

%% carica a meta prima prova 19/07/2019
plot_tensioni(carica19_07_19_a_meta,'carica a meta prima prova 19/07/2019')

%% carica a meta seconda prova 19/07/2019
plot_tensioni(carica19_07_19_a_meta_2,'carica a meta seconda prova 19/07/2019')

%% carica a meta senza reset 19/07/2019
plot_tensioni(carica19_07_19_a_meta_senza_reset,'carica a meta senza reset 19/07/2019')

%% carica a meta con reset e delay 19/07/2019
plot_tensioni(carica19_07_19_a_meta_con_reset_e_delay,'carica a meta con reset e delay 19/07/2019')

%% carica a metà con reset e delay da 50ms 19/07/2019
plot_tensioni(carica19_07_19_a_meta_con_reset_e_delay_da50ms,'carica a metà con reset e delay da 50ms 19/07/2019')

%% scarica 1.6A 19/07/19
plot_tensioni(scarica19_07_19,'scarica 1.6A 19/07/19')

%% scarica 1.6A 31/07/19
plot_tensioni(scarica31_07_19,'scarica 1.6A 31/07/19')

%% carica 2A 01/08/19 con temperature
plot_tensioni(carica01_08_19,'carica 2A 01/08/19 con temperature')

%% Scarica 1.6A 28/10/2019
plot_tensioni(scarica28_10_19,'Scarica 1.6A 28/10/2019')


%% Scarica 1.6A 28/10/2019
plot_tensioni(scarica23_10_19,'Scarica 1.6A 28/10/2019')


%% Carica 28/10/2019
plot_tensioni(carica28_10_19,'Carica 28/10/2019')


%% Carica 15/01/2020
plot_tensioni(carica15_01_20,'Carica 15/01/2020')


%% Carica 09/03/2020
% da qui in poi utilizziamo degli holder nuovi
plot_tensioni(carica09_03_20,'Carica 09/03/2020')


function plot_tensioni(tabella_tensioni,titolo)
    figure();
    
    for i=1:15
        cella_i=append('cella',int2str(i-1));
        p(i)=plot(tabella_tensioni.Tempo,tabella_tensioni.(cella_i),'DisplayName',cella_i);
        p(i).DataTipTemplate.DataTipRows(end+1) = dataTipTextRow('',repmat({p(i).DisplayName},size(p(i).XData)));
        p(i).DataTipTemplate.DataTipRows(1).Label = 'Secondi';
        p(i).DataTipTemplate.DataTipRows(2).Label = 'Tensione';
        hold on
    end
    
    title(titolo)
    xlabel('Secondi')
    ylabel('Tensione')
    clear cella_i i p;
    
    %controllo se è possibile plottare temperatura
    Exist_Column = strcmp('ntc0',tabella_tensioni.Properties.VariableNames);
    val = Exist_Column(Exist_Column==1) ;
    if val
        yyaxis right
        p(16)=plot(tabella_tensioni.Tempo,tabella_tensioni.ntc0);
        ylabel('Temperatura')
        p(16).DataTipTemplate.DataTipRows(1).Label = 'Secondi';
        p(16).DataTipTemplate.DataTipRows(2).Label = 'Temperatura';
    end
end