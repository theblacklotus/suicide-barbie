include( "../gameBase.lua" )

level0_data = {
	{
		name = "testObj:testCyl:pCylinder1",
		entityClass = StaticEntity
		,visComponent = {
			mesh = r_mesh("testCyl.x"),
			effect = r_effect("base.fx"),
			textures = {
				diffuse = r_texture("Sample.dds")
			}
		}
		,transform = {
			origin = vec3(-0.03115058998, 0, 6.521890477),
			rotationAndScale = matrix3x3(
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec3(0, 0, 1))
		}
		,data = {
			 moo = 0
			,boo = 0
			,zoo = 0
			,name = "hello"
			,parent_name = "moo"
			,xoo = 0
			,foo = 0
			,loo = ""
			,hey = false
			,hooray = ""
		}
	} -- /testObj:testCyl:pCylinder1
	,{
		name = "testObj:testTorus:pTorus1",
		entityClass = StaticEntity
		,visComponent = {
			mesh = r_mesh("testTorus.x"),
			effect = r_effect("base.fx"),
			textures = {
				n = 0
			}
		}
		,transform = {
			origin = vec3(-0.03115058998, 0.750319, 6.521890477),
			rotationAndScale = matrix3x3(
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec3(0, 0, 1))
		}
		,data = {
			 moo = 0
			,boo = 0
			,zoo = 0
			,value2 = 0
			,name = "I'm torus"
			,value = 0
			,moo = ""
			,boo = 0
		}
	} -- /testObj:testTorus:pTorus1
	,{
		name = "testObj1:testCyl:pCylinder1",
		entityClass = StaticEntity
		,visComponent = {
			mesh = r_mesh("testCyl.x"),
			effect = r_effect("base.fx"),
			textures = {
				diffuse = r_texture("Sample.dds")
			}
		}
		,transform = {
			origin = vec3(5.578623265, 0, 0),
			rotationAndScale = matrix3x3(
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec3(0, 0, 1))
		}
		,data = {
			 name = "hello"
			,parent_name = ""
		}
	} -- /testObj1:testCyl:pCylinder1
	,{
		name = "testObj1:testTorus:pTorus1",
		entityClass = StaticEntity
		,visComponent = {
			mesh = r_mesh("testTorus.x"),
			effect = r_effect("base.fx"),
			textures = {
				diffuse = r_texture("earth.dds")
			}
		}
		,transform = {
			origin = vec3(5.578623265, 0.750319, 0),
			rotationAndScale = matrix3x3(
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec3(0, 0, 1))
		}
		,data = {
			 value2 = 0
			,name = "I'm torus"
			,value = 0
		}
	} -- /testObj1:testTorus:pTorus1
	,{
		name = "testObj2:testCyl:pCylinder1",
		entityClass = StaticEntity
		,visComponent = {
			mesh = r_mesh("testCyl.x"),
			effect = r_effect("base.fx"),
			textures = {
				diffuse = r_texture("Sample.dds")
			}
		}
		,transform = {
			origin = vec3(-0.05339224353, 4.864771288, -0.02844195537),
			rotationAndScale = matrix3x3(
				vec3(0.6214520828, -0.7769669881, -0.1005962632),
				vec3(0.7344623775, 0.5330773691, 0.419992303),
				vec3(-0.2726945634, -0.3348892621, 0.9019350626))
		}
		,data = {
			 name = "hello"
			,parent_name = ""
			,moo = ""
			,mooo = 1
		}
	} -- /testObj2:testCyl:pCylinder1
	,{
		name = "testObj2:testTorus:pTorus1",
		entityClass = StaticEntity
		,visComponent = {
			mesh = r_mesh("testTorus.x"),
			effect = r_effect("base.fx"),
			textures = {
				diffuse = r_texture("earth.dds")
			}
		}
		,transform = {
			origin = vec3(0.2666887603, 5.52162304, 0.1420647249),
			rotationAndScale = matrix3x3(
				vec3(0.6214520828, -0.7769669881, -0.1005962632),
				vec3(0.7344623775, 0.5330773691, 0.419992303),
				vec3(-0.2726945634, -0.3348892621, 0.9019350626))
		}
		,data = {
			 value2 = 1
			,name = "I'm super torus"
			,value = 0
		}
	} -- /testObj2:testTorus:pTorus1
	,{
		name = "testObjNoTex:testCyl:pCylinder1",
		entityClass = StaticEntity
		,visComponent = {
			mesh = r_mesh("testCyl.x"),
			effect = r_effect("base.fx"),
			textures = {
				n = 0
			}
		}
		,transform = {
			origin = vec3(0, 0, 0),
			rotationAndScale = matrix3x3(
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec3(0, 0, 1))
		}
		,data = {
			 name = "hello"
		}
	} -- /testObjNoTex:testCyl:pCylinder1
	,{
		name = "testObjNoTex:testTorus:pTorus1",
		entityClass = StaticEntity
		,visComponent = {
			mesh = r_mesh("testTorus.x"),
			effect = r_effect("base.fx"),
			textures = {
				n = 0
			}
		}
		,transform = {
			origin = vec3(0, 0.750319, 0),
			rotationAndScale = matrix3x3(
				vec3(1, 0, 0),
				vec3(0, 1, 0),
				vec3(0, 0, 1))
		}
		,data = {
			 value2 = 0
			,name = "I'm torus"
			,value = 0
		}
	} -- /testObjNoTex:testTorus:pTorus1
}
-- /level0_data
return level0_data
