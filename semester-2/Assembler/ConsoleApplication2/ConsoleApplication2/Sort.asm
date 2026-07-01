.686
.model flat, C
.stack 
.data

.CODE

PUBLIC Sort

Sort PROC
            push ebp
            mov ebp, esp

            push ebx
            push esi
            push edi

            mov edi, [ebp+8]
            mov ecx, [ebp+12]
            dec ecx

            mov esi, edi
            add esi, 4

        for1_ :
            mov eax, [esi]
            mov ebx, esi
            sub ebx, 4

        for2_ :
            cmp ebx, edi
            jl skip_

            cmp DWORD PTR [ebx], eax
            jle skip_

            mov edx, [ebx]
            mov [ebx+4], edx

            sub ebx, 4
            jmp for2_

        skip_ :
            mov [ebx+4], eax

            add esi, 4
            loop for1_

            pop edi
            pop esi
            pop ebx

            pop ebp
            ret
Sort ENDP

END