modded class PluginRecipesManagerBase extends PluginBase
{
	override void RegisterRecipies()
	{
		RegisterRecipe(new Craft_50cm_test_wall_Kit);
		super.RegisterRecipies();
	}
};