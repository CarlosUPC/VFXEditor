Shader ""Umbra/ShaderName""
	{
		Properties{
		//Properties
		}

		SubShader
		{
		Tags { ""RenderType"" = ""Transparent"" ""Queue"" = ""Transparent"" }
		
		Pass
		{
			ZWrite Off
			Blend SrcAlpha OneMinusSrcAlpha
		
			CGPROGRAM
		
			#pragma vertex vert
			#pragma fragment frag
			#include ""UnityCG.cginc""
		
			struct VertexInput {
				float4 vertex : POSITION;
				float2 uv:TEXCOORD0;
				float4 tangent : TANGENT;
				float3 normal : NORMAL;
		
				//VertexInput
			};

			struct VertexOutput {
				float4 pos : SV_POSITION;
				float2 uv:TEXCOORD0;

				//VertexOutput
			};
		
			//Variables
		
			//Functions
		
			VertexOutput vert (VertexInput v)
			{
				VertexOutput o;
				o.pos = UnityObjectToClipPos (v.vertex);
				o.uv = v.uv;

				//VertexFactory

				return o;
			}


			fixed4 frag(VertexOutput i) : SV_Target
			{

				//MainImage

			}

			ENDCG
		}
	  }
	}
	