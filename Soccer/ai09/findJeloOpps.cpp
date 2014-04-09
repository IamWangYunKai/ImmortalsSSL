#include "ai09.h"

int ai09::findJeloOpps ( float minX , int* ans , int mask1 , int mask2 , bool acceptNearBall , bool acceptGooshe )
{
	int index = 0;
	for ( int i = 0 ; i < 12 ; i ++ )
	{
        if ( i == oppGK )
            continue;
		if ( i == mask1 || i == mask2 )
			continue;
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( ( fabs ( OppRobot[i].Position.X ) > 3025 ) ||
			( fabs ( OppRobot[i].Position.Y ) > 2025 ) )
			continue;
		if ((!acceptNearBall)&&(( DIS ( ball.Position , OppRobot[i].Position ) < 500 ))) {
			continue;
		}
		if ((!acceptGooshe)&&(isGooshe(i,0)))
			continue;
		//if (isGooshe(i,1))
		//	continue;

		if ( OppRobot[i].Position.X*side > minX )
		{
			ans[index] = i;
			index ++;
		}
	}
	
	for (int i = index; i<12; i++) {
		ans[i] = -1;
	}
	
	return index;
}