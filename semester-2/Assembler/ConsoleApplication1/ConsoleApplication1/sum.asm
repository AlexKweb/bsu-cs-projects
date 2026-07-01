.CODE

Sum PROC
			cmp ecx, edx
			jl else1_
			mov eax, ecx
			ret
	else1_: 
			mov eax, edx
			ret
Sum ENDP

END