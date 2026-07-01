.686
.model flat, C
.stack 
.data

.CODE

Sort PROC
            push ebp
            mov ebp, esp

            mov edi, [ebp+8]       
            mov ecx, [ebp+12]   
            dec ecx

        for1_ :
            mov eax, [edi]
            mov edx, edi

            mov esi, edi
            add esi, 4

            mov ebx, ecx

        for2_ :
            cmp DWORD PTR [esi], eax
            jge skip_

            mov eax, [esi]
            mov edx, esi

        skip_ :
            add esi, 4
            dec ebx
            cmp ebx, 0
            jne for2_

            mov ebx, [edi]
            mov [edi], eax
            mov [edx], ebx

            add edi, 4
            loop for1_

            pop ebp
            ret
Sort ENDP

END