class CfgVehicles
{
	class Armband_ColorBase;
	class Armband_EPOCH: Armband_ColorBase
	{
		scope = 2;
		author = "DirtySanchez";
		displayName = "EPOCH Armband";
		descriptionShort = "Epoch Rocks!";
		vehicleClass = "Epoch_Clothing";
		visibilityModifier = 0.85;
		color = "Epoch";
		hiddenSelectionsTextures[] = {"\emt\dzsa_epoch_assets\DS\Armbands\armbend_epoch_g_co.paa","\emt\dzsa_epoch_assets\DS\Armbands\armbend_small_epoch_co.paa","\emt\dzsa_epoch_assets\DS\Armbands\armbend_small_epoch_co.paa","\emt\dzsa_epoch_assets\DS\Armbands\armbend_big_epoch_co.paa","\emt\dzsa_epoch_assets\DS\Armbands\armbend_big_epoch_co.paa"};
	};
	class WeldingMask;
	class WeldingMask_DevilsCamo: WeldingMask
	{
		scope = 2;
		author = "DirtySanchez";
		displayName = "DevilsCamo Welding Mask";
		color = "DevilsCamo";
		vehicleClass = "Epoch_Headgear";
		visibilityModifier = 0.85;
		noMask = 1;
		hiddenSelectionsTextures[] = {"\emt\dzsa_epoch_assets\DS\Masks\welding_mask_devilscamo_co.paa","\emt\dzsa_epoch_assets\DS\Masks\welding_mask_devilscamo_co.paa","\emt\dzsa_epoch_assets\DS\Masks\welding_mask_devilscamo_co.paa"};

	};
///walls
class Inventory_Base;
class FenceKit;
class FenceKitPlacing;
	class test_wall_Kit: FenceKit
	{
		scope=2;
		displayName="Test Wall Kit";
		descriptionShort="Test Wall Kit";
	};
	class test_wall_Placing: FenceKitPlacing
	{
		scope=2;
		model="\DZ\gear\camping\fence_kit_placing.p3d";
		hiddenSelections[]=
		{
			"placing"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\gear\consumables\data\pile_of_planks_co.tga"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\gear\camping\data\fence_pile_of_planks.rvmat"
		};
		hologramMaterial="tent_medium";
		hologramMaterialPath="emt\dzsa_epoch_pv_assets\emt\baseb\walls\data";/// change this
		slopeLimit=0.25;
	};
	class test_Wall: Inventory_Base
	{
		scope=2;
		displayName="Test_Wall";
		descriptionShort="Test_Wall";
		//model="\DZ\gear\camping\fence.p3d";
		model="\emt\dzsa_epoch_pv_assets\emt\baseb\walls\wall_1m.p3d";
		bounding="BSphere";
		overrideDrawArea="3.0";
		forceFarBubble="true";
		SingleUseActions[]={527};
		ContinuousActions[]={155,239};
		InteractActions[]={1051};
		armAction="Disarm";
		handheld="false";
		lootCategory="Crafted";
		carveNavmesh=1;
		weight=3000;
		itemSize[]={2,3};
		useEntityHierarchy="true";
		physLayer="item_large";
		rotationFlags=2;
		class test_Wall : Inventory_Base
		{
			scope = 2;
			displayName = "Test_Wall";
			descriptionShort = "Test_Wall";
			//model="\DZ\gear\camping\fence.p3d";
			model = "\emt\dzsa_epoch_pv_assets\emt\baseb\walls\wall_1m.p3d";
			bounding = "BSphere";
			overrideDrawArea = "3.0";
			forceFarBubble = "true";
			SingleUseActions[] = { 527 };
			ContinuousActions[] = { 155,239 };
			InteractActions[] = { 1051 };
			armAction = "Disarm";
			handheld = "false";
			lootCategory = "Crafted";
			carveNavmesh = 1;
			weight = 3000;
			itemSize[] = { 2,3 };
			useEntityHierarchy = "true";
			physLayer = "item_large";
			rotationFlags = 2;
			class DamageSystem
			{
				class GlobalHealth
				{
					class Health
					{
						hitpoints = 100;
					};
				};
				class GlobalArmor
				{
					class Projectile
					{
						Health = 0;
						Blood = 0;
						Shock = 0;
					};
				};
			};
			attachments[] = {};
			class AnimationSources
			{
				class AnimSourceShown
				{
					source = "user";
					animPeriod = 0.0099999998;
					initPhase = 0;
				};
				class AnimSourceHidden
				{
					source = "user";
					animPeriod = 0.0099999998;
					initPhase = 1;
				};
				class AnimRotate
				{
					source = "user";
					animPeriod = 0.0099999998;
					initPhase = 0;
				};
				class Wall_Interact_Rotate : AnimRotate
				{
				};
				class Wall_Barbedwire_Mounted_Rotate : AnimRotate
				{
				};
				class Wall_Camonet_Rotate : AnimRotate
				{
				};
				class Wall_Gate_Rotate : AnimRotate
				{
				};
				class Wall_Base_Down_Rotate : AnimRotate
				{
				};
				class Wall_Base_Up_Rotate : AnimRotate
				{
				};
				class Wall_Wood_Down_Rotate : AnimRotate
				{
				};
				class Wall_Wood_Up_Rotate : AnimRotate
				{
				};
				class Wall_Metal_Down_Rotate : AnimRotate
				{
				};
				class Wall_Metal_Up_Rotate : AnimRotate
				{
				};
				class Material_WoodenLogs : AnimSourceHidden
				{
				};
				class Material_MetalWire : AnimSourceHidden
				{
				};
				class Wall_Barbedwire_1 : AnimSourceHidden
				{
				};
				class Wall_Barbedwire_1_Mounted : AnimSourceHidden
				{
				};
				class Wall_Barbedwire_2 : AnimSourceHidden
				{
				};
				class Wall_Barbedwire_2_Mounted : AnimSourceHidden
				{
				};
				class Wall_Camonet : AnimSourceHidden
				{
				};
				class Deployed : AnimSourceHidden
				{
				};
				class Base : AnimSourceHidden
				{
				};
				class Wall_Platform : AnimSourceHidden
				{
				};
				class Wall_Platform_Stairs_Left : AnimSourceHidden
				{
				};
				class Wall_Platform_Stairs_Right : AnimSourceHidden
				{
				};
				class Wall_Gate : AnimSourceHidden
				{
				};
				class Wall_Base_Down : AnimSourceHidden
				{
				};
				class Wall_Base_Up : AnimSourceHidden
				{
				};
				class Wall_Metal_Down : AnimSourceHidden
				{
				};
				class Wall_Metal_Up : AnimSourceHidden
				{
				};
				class Wall_Wood_Down : AnimSourceHidden
				{
				};
				class Wall_Wood_Up : AnimSourceHidden
				{
				};
			};
			class Construction {};

			class GUIInventoryAttachmentsProps {};
			class PointLights {};
		};
	};

//inventory wall
	class INV_test_Wall: Inventory_Base
	{
	    scope = 2;
	    displayName = "INV_test_Wall";
	    descriptionShort = "INV_test_Wall";
	    model = "emt\dzsa_epoch_pv_assets\emt\baseb\walls\wall_1m.p3d";
	    ContinuousActions[] = {148,147};
	    rotationFlags = 17;
	    weight = 510;
	    itemSize[] = {3,2};
	    repairableWithKits[] = {5,2};
	    repairCosts[] = {30.0,25.0};
	    //hiddenSelections[] = {"camo1"};
	    //hiddenSelectionsTextures[] = {"\emt\dzsa_epoch_pv_assets\emt\baseb\bboard_01\data\billboard_01_co.paa"};		
	    class DamageSystem
	    {
	    	class GlobalHealth
	    	{
	    		class Health
	    		{
	    			hitpoints = 100;
	    			healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
	    		};
	    	};
	    };
	    class AnimEvents{};
    };

};