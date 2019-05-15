modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);    //This executes the original so you dont mess anything up
        
        actions.Insert(ActionUnlockBarrel);
        actions.Insert(ActionLockBarrel);
    }
}