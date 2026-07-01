.686
.model flat, C
.stack
.data

.CODE

Sort PROC arr:DWORD, n:DWORD

            push ebx
            push esi
            push edi

            mov edi, arr        
            mov ecx, n
            dec ecx             

            mov esi, edi
            add esi, 4          

for1_ :
            mov eax, [esi]      
            mov ebx, esi
            sub ebx, 4         

while_ :
            cmp ebx, edi
            jl insert_

            cmp [ebx], eax
            jle insert_

            mov edx, [ebx]
            mov [ebx+4], edx

            sub ebx, 4
            jmp while_

insert_ :
            mov [ebx+4], eax

            add esi, 4
            loop for1_

            pop edi
            pop esi
            pop ebx
            ret

Sort ENDP

END