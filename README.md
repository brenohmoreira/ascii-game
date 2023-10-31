## SPACE INVADER - ASCII 

1. Cadastro de player: nome  
    1.1 Guardar informações do player em arquivo para posterior ranking
    1.2 Salvar score do player
    1.3 Listar players com maiores score (?)

2. Lógica 
    2.1. Matriz - Estrutura e player
        2.1.1. 3 primeiras linhas de monstros;
        2.1.2. linha no extremo inferior (^) é o player
        2.1.3. while(1) pegando caractere
            2.1.3.1. arrow left e arrow right mudando posição ^ na matriz +1 na column ou -1 na column
            2.1.3.2. space dando disparo
            2.1.3.3. tudo isso como uma rotina de thread
    2.2. Matriz - Ataques inimigos
        2.2.1. Thread para cada ataque
            2.2.1.1. Sorteio de um número de 0 a n a cada x segundos
            2.2.1.2. Verifica se existe inimigo no local
                2.2.1.2.1. Se não existe, verifica se há um antes dele. Se não tiver, sorteia outro na primeira row
            2.2.1.3. Cria thread e rotina de ataque
                2.2.1.3.1. Quando chega na última linha, verifica se o ataque colide com (^) / player
                    2.2.1.3.1.1. Se sim, fim de jogo
                    2.2.1.3.1.2. Se não, score aumenta
    
