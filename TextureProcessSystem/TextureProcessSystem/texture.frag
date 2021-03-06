 #version 330
uniform sampler2D texs[55];
uniform vec4 diffuse;
in vec4 positionV;
in vec2 texCoordV1;
in vec2 texCoordV2;
in vec2 texCoordV3;
in vec2 texCoordV4;
in vec2 texCoordV5;
in vec2 texCoordV6;
in vec2 texCoordV7;
in vec2 texCoordV8;
in vec2 texCoordV9;
in vec2 texCoordV10;
in vec2 texCoordV11;
in vec2 texCoordV12;

in vec4 texID_1V;
in vec4 texID_2V;
in vec4 normalsV;
out vec4 colorOut;
out vec4 positionV2;

void main() {
	vec2 texCoordVS[16];
	texCoordVS[0] = texCoordV1;
	texCoordVS[1] = texCoordV2;
	texCoordVS[2] = texCoordV3;
	texCoordVS[3] = texCoordV4;
	texCoordVS[4] = texCoordV5;
	texCoordVS[5] = texCoordV6;
	texCoordVS[6] = texCoordV7;
	texCoordVS[7] = texCoordV8;
	texCoordVS[8] = texCoordV9;
	texCoordVS[9] = texCoordV10;
	texCoordVS[10] = texCoordV11;
	texCoordVS[11] = texCoordV12;

	colorOut=vec4(0,0,0,1);

	int num=0;
	int k=0;
	int index[16];
	float alpha[16];
	int layers=0;
	for(int i=0;i<8;i++)
	{
		if(i<4)
		{
			k=int(texID_1V[i]);
		}else if(i<8)
		{
			k=int(texID_2V[i-4]);
		}
		if(k>=0)
		{
			index[num]=k;						
			alpha[num]=1;
			vec4 color1=texture2D(texs[k], texCoordVS[num]);
			if(i>0)
			{
				if((color1[0]==1)&&(color1[1]==1)&&(color1[2]==1))
				{
					alpha[num]=0;
				}else
				{
					layers++;
				}
			}else
			{
				layers++;
			}
			num++;
		}
	}

	for(int i=0;i<num;i++)
	{
		int j=index[i];
		float alphaL=1;
		if(num==1||layers<=1)
		{
			alphaL=alpha[i];
		}else
		{
			if(j==0)
			{
				alphaL=0.1;
			}else
			{
				alphaL=alpha[i]*0.9;
			}
		}
		vec4 color=texture2D(texs[j], texCoordVS[i]);
		if((color[0]>0.7)&&(color[1]>0.7)&&(color[2]>0.7)&&(i!=0))
		{
			//colorOut=color*0.9;
			//colorOut-=vec4(0.3,0.3,0.3,0);
		}else
		{
			colorOut=color*0.9;
			colorOut-=vec4(0.3,0.3,0.3,0);
			
		}
		//colorOut=color*0.9;
	}
	
	//colorOut+=(texture2D(texs[0], texCoordVS[0]));
	float power=1;
	power=max(dot((diffuse-positionV),normalsV),0);
	colorOut=vec4(0.1,0.1,0.1,1)*power+colorOut;
	positionV2=positionV;

}