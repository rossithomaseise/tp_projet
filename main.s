	.data
erreur_ouverture_fichier:	.asciiz "erreur ouverture fichier\n"
erreur_ouverture_fichier2:	.asciiz "\nerreur ouverture fichier partie 2\n"
erreur_ouverture_fichier3:	.asciiz "\nerreur ouverture fichier 3\n"
success_ouverture_fichier:	.asciiz "succès ouverture fichier\n"
success_ouverture_fichier2:	.asciiz "\nsuccès ouverture fichier partie 2\n"
success_ouverture_fichier3:	.asciiz "\nsuccès ouverture fichier 3\n"
nbr_de_phrases:			.asciiz "le nombre de phrase est: "
nbr_mots:			.asciiz "le nombre de mot est: "
nbr_de_lignes:			.asciiz "le nombre de lignes est: "
nbr_de_pages:			.asciiz "le nombre de pages est: "
nbr_lignes_dans_page:		.word 6

chemin_texte:                	.asciiz "/home/polytech/Téléchargements/Projet_Archi_final/Marseillaise.txt"
chemin_texte2:                	.asciiz "/home/polytech/Téléchargements/Projet_Archi_final/texte2.txt"
buffer:				.space 1
tab_resultat_fonction:		.word 0,0,0,0

#variables fonction fréquence mot
caractereActuel:		.word 0
lastcaratere:			.word 0
loop:				.word 0
#nombre de répétitons des mots les plus fréquents associés à leur position dans TabChar
TabInt:				.word 0,0,0,0,0,0,0,0,0,0
#tableau de taille 28 pour sauvegarder les mots les plus fréquents
temp0			:	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp1:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp2:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp3:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp4:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp5:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp6:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp7:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp8:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp9:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#char* TabChar[10] = {temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9};
temp:				.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp_char:			.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
temp_int:			.word 0
#tour:				.word 0
existe:				.word 0
min:				.word 999
indiceMin:			.word 0
comp:				.word 0
    .text
main:
	subu $sp,$sp,8    			# prologue
	sw $fp,4($sp)
	addi $fp,$sp,8
	
    	li  $v0, 13            	 		# syscall pour l'ouverture du fichier
        la  $a0, chemin_texte  			# chemin du fichier
        li  $a1,0        	 		# lecture = 0 (flag)
        li  $a2,0        	 		# = 0 (mode : reading from standard input)
        syscall        		 		# ouverture du fichier
	move $s6, $v0      			# sauvegarde adresse fichier 
	
        # erreur d'ouverture du fichier si $vo est négatif
        bltz $v0,erreur_ouverture_part1		# redirection fin du programme
        la $a0,success_ouverture_fichier	# affichage du texte
   	ori $v0,$zero,4				
        syscall
        
        la $t6,tab_resultat_fonction		# chargement du tableau pour resultat fonction
        
        ori $s7,$zero,0				# var "caractère_précédent" de la fonction nb_mot initialisé à 0
        
        jal f1_lecture_caractere 		# appel fonction 
  

        j fin_programme_part1

	erreur_ouverture_part1:	
	la $a0,erreur_ouverture_fichier		# affichage du texte
	ori $v0,$zero,4				
        syscall

	fin_programme_part1:
	li   $v0, 16       			# syscall pour fermer le fichier
  	move $a0, $s6      			# adresse fichier dans $a0 pour fermeture
  	syscall            			# fermeture
  	
  	
	# On répète la procédure d'ouverture et de fermeture du fichier afin de lancer la fonction fréquence mot  	
  	li  $v0, 13            	 		# syscall pour l'ouverture du fichier
        la  $a0,chemin_texte  			# chemin du fichier
        li  $a1,0        	 		# lecture = 0 (flag)
        li  $a2,0        	 		# = 0 (mode : reading from standard input)
        syscall        		 		# ouverture du fichier
	move $s6, $v0      			# sauvegarde adresse fichier 
	
        # erreur d'ouverture du fichier si $vo est négatif
        bltz $v0,erreur_ouverture_part2		# redirection fin du programme
        la $a0,success_ouverture_fichier2	# affichage du texte
   	ori $v0,$zero,4				
        syscall
        
        jal f7_frequence_mot

        
        j fin_programme_part2

	erreur_ouverture_part2:	
	la $a0,erreur_ouverture_fichier2	# affichage du texte
	ori $v0,$zero,4				
        syscall

	fin_programme_part2:
	li   $v0, 16       			# syscall pour fermer le fichier
  	move $a0, $s6      			# adresse fichier dans $a0 pour fermeture
  	syscall            			# fermeture
  	

	lw $fp,4($sp)				# fin de l'appel de la fonction main
	addu $sp,$sp,8
	
        ori $v0,$zero,10			# sortie du programme
        syscall
        
        
f1_lecture_caractere:  				# lecture du fichier caractère par caractère 	
        subu $sp,$sp,12    			# prologue
	sw $fp,4($sp)
	sw $ra,8($sp)
	addu $fp,$sp,12
	
	lecture_caractere_f1:
        li $v0,14				# code service lecture fichier
        
        #chargement des paramètres
        ori $a0,$s6,0				# adresse fichier
        la $a1,buffer				# adresse buffer
        ori $a2,$zero,1				# nombre de caractère lue
        syscall
        
        ori $s0,$v0,0				# si V0= 0 => fin du fichier
        beq $s0,$zero,fin_programme_f1		# on sort si c'est le dernier caractère
        
        jal f2_compteur_phrase			
        jal f3_compteur_mot			
        jal f4_compteur_ligne
	
        ori $a0,$a1,0				# affichage du caractère
        ori $v0,$zero,4
        syscall
        
        j lecture_caractere_f1 			# on refait un tour de boucle
	
	fin_programme_f1:	
		
	jal f5_compteur_page	
	jal f6_affichage_resultat
	
	lw $ra,8($sp)				# epilogue
	lw $fp,4($sp)
	addu $sp,$sp,12
	
	jr $ra
	
f2_compteur_phrase:
	subu $sp,$sp,8   			# prologue
	sw $fp,4($sp)
	addu $fp,$sp,8
	
	ori $t0,$zero,0x21			# caract '!'
	ori $t1,$zero,0x2E			# caract '.'
	ori $t2,$zero,0x3F			# caract '?'
	
	lb $s5,0($a1)				# on récupoère le caractère lu
	
	beq $s5,$t0,compteur_phrase_f2	 	# si = '!'
	beq $s5,$t1,compteur_phrase_f2 		# si = '.'
	beq $s5,$t2,compteur_phrase_f2		# si = '?'
	j fin_programme_f2	
	
	compteur_phrase_f2:			# nb phrase + 1
	addi $s1,$s1,1
	
	fin_programme_f2:			# epilogue
	lw $fp,4($sp)
	addu $sp,$sp,8
	
	sw $s1,0($t6)				# on écrit dans le tab_resultat
	
	jr $ra

f3_compteur_mot:
	subu $sp,$sp,8   			# prologue
	sw $fp,4($sp)
	addu $fp,$sp,8

	ori $t0,$zero,0x21			# caract '!'
	ori $t1,$zero,0x2E			# caract '.'
	ori $t2,$zero,0x3F			# caract '?'
	ori $t3,$zero,0x20			# caract ' '
	ori $t4,$zero,0x2C			# caract ','
	ori $t5,$zero,0x3B			# caract ';'
	ori $t7,$zero,0x0A			# caract '\n'
	
	lb $s5,0($a1)				# on récupère le caractère lu
	
	beq $s5,$t1,compteur_mot_2_f3 		# if == '.'
	beq $s5,$t3,compteur_mot_2_f3		# if == ' '
	beq $s5,$t7,compteur_mot_1_f3		# if == '\n'
	j fin_programme_f3			# sinon on sort du programme
	
	compteur_mot_1_f3:			# comparaison avec $s7 = caractère_précédent
	beq $s7,$t2,fin_programme_f3		# if == '?'
	beq $s7,$t0,fin_programme_f3		# if == '!'
	beq $s7,$t7,fin_programme_f3		# if == '\n'
	beq $s7,$t1,fin_programme_f3		# if == '.'
	beq $s7,$t4,fin_programme_f3		# if == ','
	beq $s7,$t5,fin_programme_f3		# if == ';'
	
	addi $s2,$s2,1				# NbMots +=1
	j fin_programme_f3
	
	compteur_mot_2_f3:
	beq $s7,$t0,fin_programme_f3		# if == '!'
	beq $s7,$t2,fin_programme_f3		# if == '?'
	beq $s7,$t3,fin_programme_f3		# if == ' '
	beq $s7,$t5,fin_programme_f3		# if == ';'
	
	addi $s2,$s2,1				# nb_mots + 1
	
	j fin_programme_f3
	
	fin_programme_f3:			# epilogue
	
	ori $s7,$s5,0				# caractère_précédent = caractere_Actuel
	sw $s2,4($t6)				# on écrit dans le tab_resultat
	lw $fp,4($sp)
	addu $sp,$sp,8
	jr $ra
	
f4_compteur_ligne:
	subu $sp,$sp,8   			# prologue
	sw $fp,4($sp)
	addu $fp,$sp,8
	
	ori $t0,$zero,0x0A			# saut de ligne '\n'
	
	lb $s5,0($a1)				# on récupoère le caractère lu
	
	beq $s5,$t0,compteur_ligne_f4		# si ='\n'
	
	j fin_programme_f4	
	
	compteur_ligne_f4:
	addi $s3,$s3,1
	
	fin_programme_f4:			# epilogue
	
	lw $fp,8($sp)
	addu $sp,$sp,8
	sw $s3,8($t6)				# on écrit dans le tab_resultat
	jr $ra
	
f5_compteur_page:
	subu $sp,$sp,8   			# prologue
	sw $fp,4($sp)
	addu $fp,$sp,8
	
        lw $t3,8($t6)				# nombre de lignes
        la $t7,nbr_lignes_dans_page		# nombre de lignes par page 
        lw $t7,0($t7)
        div $t3,$t7				
        mfhi $t5				# reste
 	mflo $t4 				# quotient
        	
        beq $t5,$zero,fin_programme_f5		
       	addi $t4,$t4,1				# +1 page si le reste est non nul


	fin_programme_f5:			# epilogue
	
	lw $fp,4($sp)				
	addu $sp,$sp,8
	sw $t4,12($t6)				# on écrit dans le tab_resultat	
	jr $ra
		
f6_affichage_resultat:
	subu $sp,$sp,8   			# prologue
	sw $fp,4($sp)
	addu $fp,$sp,8
	
	ori $a0,$zero,0x0A			# affichage saut de ligne	
    	ori $v0,$zero,11
        syscall	
		
	la $a0,nbr_de_phrases			# affichage texte nbr phrases
    	ori $v0,$zero,4
        syscall
        
	lw $s3,0($t6)				# affichage du tab_resultat 1
	ori $a0,$s3,0
	ori $v0,$zero,1
	syscall
	
	ori $a0,$zero,0x0A			# affichage saut de ligne	
    	ori $v0,$zero,11
        syscall	
        
	la $a0,nbr_mots				#affichage texte nbr mots
    	ori $v0,$zero,4
        syscall
        
      
	lw $s3,4($t6)				# affichage du tab_resultat 2
	ori $a0,$s3,0
	ori $v0,$zero,1
	syscall
	
	ori $a0,$zero,0x0A			# affichage saut de ligne	
    	ori $v0,$zero,11
        syscall
	
	la $a0,nbr_de_lignes			#afficher le texte nbr lignes
    	ori $v0,$zero,4
        syscall
	
	
	lw $s3,8($t6)				# affichage du tab_resultat 3
	ori $a0,$s3,0
	ori $v0,$zero,1
	syscall
	
		
	ori $a0,$zero,0x0A			# affichage saut de ligne	
    	ori $v0,$zero,11
        syscall
        
        #calcule et affichage nbr pages
        la $a0,nbr_de_pages			#affiche message pages
    	ori $v0,$zero,4
        syscall

	lw $s3,12($t6)				# affichage du tab_resultat 4
	ori $a0,$s3,0
	ori $v0,$zero,1
	syscall


	lw $fp,4($sp)				# epilogue
	addu $sp,$sp,8
	jr $ra

f7_frequence_mot:
	subu $sp,$sp,8   			# prologue
	sw $fp,4($sp)
	addu $fp,$sp,8
	
	#ATTENTION $t0 et $t7 réservé, s7 et s5 pour caractère précédent et caraactère actuel
	
	
	la $t7,tab_resultat_fonction		# chargement nb mots texte
	lw $t7,4($t7)
	ori $t0,$zero,0				# initialisation i alias big (code C)
	#1er for
	grand_tour_f7:
	bge $t0,$t7,fin_programme_f7
	
	#2ème for = initialisation de temp
	la $t6,temp				# chargement du tableau temp
	ori $t1,$zero,0				# initialisation i alias i2 (code C)
	addi $t5,$zero,28			# nb éléments de temp = 28
	addi $t2,$zero,4			# valeur 4
	
	initialisation_temp_f7:
	bge $t1,$t5,fin_initialisation_temp_f7
	mul $t3,$t2,$t1				# 4*i
	add $t4,$t6,$t3				# récupère adresse temp[i]
	lw $s1,0($t4)				# récupère valeur de l'adresse temp[i]
	ori $s1,$zero,0				# initialisation à zéro
	sw $s1,0($t4)
	addi $t1,$t1,1				# incrémentation 2ème for init temp
	j initialisation_temp_f7		# retour 2ème for alias	
	
	fin_initialisation_temp_f7:		# fin initialisation temp
	# initialisation loop
	la $t6,loop				# chargement de l'entier loop
	lw $s1,0($t6)
	ori $s1,$zero,0				# initialisation loop à zéro
	sw $s1,0($t6)	
			
	# lecture caractère
        li $v0,14				# code service lecture fichier
        
        # chargement des paramètres
        ori $a0,$s6,0				# adresse fichier
        la $a1,buffer				# adresse buffer
        ori $a2,$zero,1				# nombre de caractère lue
        syscall
        
        ori $s7,$v0,0				# copie du caractère lu dans Caractère_précédent
        
       	# 3ème for
	ori $t1,$zero,0				# initialisation i alias i (code C)	
	addi $t2,$zero,4			# valeur 4
	
	ori $t3,$zero,0x21			# caract '!'
	ori $t4,$zero,0x2E			# caract '.'
	ori $t5,$zero,0x3F			# caract '?'
	ori $t6,$zero,0x20			# caract ' '
	ori $s1,$zero,0x2C			# caract ','
	ori $s2,$zero,0x3B			# caract ';'
	ori $s3,$zero,0x0A			# caract '\n'
	
	# si ponctuation ou espace collé, on les ignore et on lie de nouveau un entier
	for_3_f7:
	bge $t1,$t2,fin_for_3_f7
	#if caractère_précédent = alors on lie un nouvelle entier
	bne $s7,$t3,suite_for_3_f7		# si != '!'
	bne $s7,$t4,suite_for_3_f7		# si != '.'
	bne $s7,$t5,suite_for_3_f7		# si != '?'
	bne $s7,$t6,suite_for_3_f7		# si != ' '
	bne $s7,$s1,suite_for_3_f7		# si != ','
	bne $s7,$s2,suite_for_3_f7		# si != ';'
	bne $s7,$s3,suite_for_3_f7		# si != '\n'
	
	# lecture caractère (fgetc)
        li $v0,14				# code service lecture fichier
        # chargement des paramètres
        ori $a0,$s6,0				# adresse fichier
        la $a1,buffer				# adresse buffer
        ori $a2,$zero,1				# nombre de caractère lue
        syscall
        ori $s7,$a1,0				# copie du caractère lu dans Caractère_précédent
	
	suite_for_3_f7:
	addi $t1,$t1,1				# incrémentation 3eme for
	j for_3_f7
	fin_for_3_f7:
	
	ori $a0,$a1,0				# affichage du caractère
        ori $v0,$zero,4
        syscall
        
	while_1_f7:
	ori $t3,$zero,0x21			# caract '!'
	ori $t4,$zero,0x2E			# caract '.'
	ori $t5,$zero,0x3F			# caract '?'
	ori $t6,$zero,0x20			# caract ' '
	ori $s1,$zero,0x2C			# caract ','
	ori $s2,$zero,0x3B			# caract ';'
	ori $s3,$zero,0x0A			# caract '\n'
	
	beq $s7,$t3,fin_while_1_f7		# si = '!'
	beq $s7,$t4,fin_while_1_f7		# si = '.'
	beq $s7,$t5,fin_while_1_f7		# si = '?'
	beq $s7,$t6,fin_while_1_f7		# si = ' '
	beq $s7,$s1,fin_while_1_f7		# si = ','
	beq $s7,$s2,fin_while_1_f7		# si = ';'
	beq $s7,$s3,fin_while_1_f7		# si = '\n'
	
	# lecture caractère (fgetc)
        li $v0,14				# code service lecture fichier
        # chargement des paramètres
        ori $a0,$s6,0				# adresse fichier
        la $a1,buffer				# adresse buffer
        ori $a2,$zero,1				# nombre de caractère lue
        syscall   
        
        ori $s0,$v0,0				# si V0= 0 => fin du fichier
        beq $s0,$zero,fin_while_1_f7		# on sort si c'est le dernier caractère
        
        ori $s5,$a1,0				# copie du caractère lu dans caractère_actuel
        la $t3,loop				
        lw $t4,0($t3)
        ori $t5,$zero,4
        mul $t5,$t4,$t5				# 4*loop
        
        la $t6,temp
       	add $s1,$t6,$t5				# adresse de tempchar + 4*loop
        lw $s2,0($s1)				# valeur de tempchar à l'adresse 4*loop
        addi $s2,$s7,0				# temp[loop] = caractere_actuel
        sw $s2,0($s1)
        ori $s7,$s5,0				# caractere_precedent = caractere_actuel
        
        addi $t4,$t4,1				# loop ++
        sw $t4,0($t3)
        
        ori $a0,$a1,0				# affichage du caractère
        ori $v0,$zero,4
        syscall
        
        
        
	j while_1_f7
   	
	
	fin_while_1_f7:
	
	
	#ATTENTION S4 UTILISE POUR LE FICHIER2
	
	
	# On re-ouvre le fichier (fichier2= fopen(..))	
  	li  $v0, 13            	 		# syscall pour l'ouverture du fichier
        la  $a0,chemin_texte2  			# chemin du fichier
        li  $a1,0        	 		# lecture = 0 (flag)
        li  $a2,0        	 		# = 0 (mode : reading from standard input)
        syscall        		 		# ouverture du fichier
	move $s4,$v0      			# sauvegarde adresse fichier 
	
        # erreur d'ouverture du fichier si $vo est négatif
        bltz $v0,erreur_ouverture_part3		# redirection fin du programme
        la $a0,success_ouverture_fichier3	# affichage du texte
   	ori $v0,$zero,4				
        syscall
        
        j fin_programme_part3
        
	erreur_ouverture_part3:	
	la $a0,erreur_ouverture_fichier3	# affichage du texte
	ori $v0,$zero,4				
        syscall

	fin_programme_part3:
	li   $v0, 16       			# syscall pour fermer le fichier
  	move $a0, $s4     			# adresse fichier dans $a0 pour fermeture
  	syscall            			# fermeture
	

	addi $t0,$t0,1				# incrémentation 1er for
	j grand_tour_f7				# retour sur le 1er for alias grand_tour
	fin_programme_f7:
	### printf
	lw $fp,4($sp)				# epilogue
	addu $sp,$sp,8
	jr $ra
