#include <iostream>
#include <string>
#include <map>
using namespace std;

map<string, string> initializePoetry() {
	map<string, string> poems;
	poems["Jaberwocky"] = "'Twas brillig, and the slithy toves \nDid gyre and gimble in the wabe :\nAll mimsy were the borogoves,\nAnd the mome raths outgrabe.\n\n"
		 "\"Beware the Jabberwock, my son!\nThe jaws that bite, the claws that catch!\nBeware the Jubjub bird, and shun\nThe frumious Bandersnatch!\"\n\n"
		 "He took his vorpal sword in hand;\nLong time the manxome foe he sought-\nSo rested he by the Tumtum tree\nAnd stood awhile in thought."
		 "And, as in uffish thought he stood,\nThe Jabberwock, with eyes of flame,\nCame whiffling through the tulgey wood,\nAnd burbled as it came!\n\n"
		 "One, two!One, two!And through and through\nThe vorpal blade went snicker - snack!\nHe left it dead, and with its head\nHe went galumphing back.\n\n"
		 "\"And hast thou slain the Jabberwock ?\nCome to my arms, my beamish boy!\nO frabjous day!Callooh!Callay!\"\nHe chortled in his joy.\n\n"
		 "'Twas brillig, and the slithy toves\nDid gyre and gimble in the wabe :\nAll mimsy were the borogoves,\nAnd the mome raths outgrabe.\n";
	poems["TheLama"] = "The one-l lama,\nHe\'s a priest.\nThe two - l llama,\nHe\'s a beast.\nAnd I will bet\nA silk pajama\nThere isn\'t any\nThree - l lllama";
	poems["Eldorado"] = "Gaily bedight, \nA gallant knight,\nIn sunshine and in shadow,\nHad journeyed long,\nSinging a song,\nIn search of Eldorado.\n\n"
		"But he grew old-\nThis knight so bold-\nAnd o\'er his heart a shadow-\nFell as he found\nNo spot of ground\nThat looked like Eldorado.\n\n"
		"And, as his strength\nFailed him at length,\nHe met a pilgrim shadow-\n\'Shadow, \' said he,\n\'Where can it be-\nThis land of Eldorado ? \'\n\n"
		"\'Over the Mountains\nOf the Moon,\nDown the Valley of the Shadow,\nRide, boldly ride, \'\nThe shade replied, -\n\'If you seek for Eldorado!\' \n";
	poems["SongOfTheOpenRoad"] = "Song of the Open Road\n		BY WALT WHITMAN\n		1\n		Afoot and light - hearted I take to the open road,\n"
		 "		Healthy, free, the world before me,\n		The long brown path before me leading wherever I choose.\n\n"
		 "		Henceforth I ask not good - fortune, I myself am good - fortune,\n		Henceforth I whimper no more, postpone no more, need nothing,\n		Done with indoor complaints, libraries, querulous criticisms,\n		Strong and content I travel the open road.\n\n"
		 "		The earth, that is sufficient,\n		I do not want the constellations any nearer,\n		I know they are very well where they are,\n		I know they suffice for those who belong to them.\n\n"
		 "		(Still here I carry my old delicious burdens,\n			I carry them, men and women, I carry them with me wherever I go,\n			I swear it is impossible for me to get rid of them,\n			I am fill'd with them, and I will fill them in return.)\n\n"
		 "		2\n		You road I enter upon and look around, I believe you are not all that is here,\n		I believe that much unseen is also here.\n\n"
		 "		Here the profound lesson of reception, nor preference nor denial,\n		The black with his woolly head, the felon, the diseas'd, the illiterate person, are not denied;\n	The birth, the hasting after the physician, the beggar's tramp, the drunkard's stagger, the laughing party of mechanics,\n	The escaped youth, the rich person's carriage, the fop, the eloping couple,\n\n"
		 "		The early market - man, the hearse, the moving of furniture into the town, the return back from the town,\n		They pass, I also pass, any thing passes, none can be interdicted,\n		None but are accepted, none but shall be dear to me.\n\n"
		 "		3\n		You air that serves me with breath to speak!\n		You objects that call from diffusion my meanings and give them shape!\n		You light that wraps me and all things in delicate equable showers!\n		You paths worn in the irregular hollows by the roadsides!\n		I believe you are latent with unseen existences, you are so dear to me.\n\n"
		 "		You flagg'd walks of the cities!you strong curbs at the edges!\n		You ferries!you planks and posts of wharves!you timber - lined sides!you distant ships!\n		You rows of houses!you window - pierc'd façades!you roofs!\n		You porches and entrances!you copings and iron guards!\n		You windows whose transparent shells might expose so much!\n		You doors and ascending steps!you arches!\n		You gray stones of interminable pavements!you trodden crossings!\n		From all that has touch'd you I believe you have imparted to yourselves, and now would impart the same secretly to me,\n		From the living and the dead you have peopled your impassive surfaces, and the spirits thereof would be evident and amicable with me.\n\n"
		 "		4\n		The earth expanding right hand and left hand,\n		The picture alive, every part in its best light,\n		The music falling in where it is wanted, and stopping where it is not wanted,\nThe cheerful voice of the public road, the gay fresh sentiment of the road.\n"
		 "		O highway I travel, do you say to me Do not leave me ?\n		Do you say Venture not-if you leave me you are lost ?\n		Do you say I am already prepared, I am well - beaten and undenied, adhere to me ?\n"
		 "		O public road, I say back I am not afraid to leave you, yet I love you,\n		You express me better than I can express myself,\n		You shall be more to me than my poem."
		 "		I think heroic deeds were all conceiv'd in the open air, and all free poems also,\n		I think I could stop here myself and do miracles,\n		I think whatever I shall meet on the road I shall like, and whoever beholds me shall like me,\n		I think whoever I see must be happy.\n\n"
		 "		5\n		From this hour I ordain myself loos'd of limits and imaginary lines,\n		Going where I list, my own master total and absolute,\n		Listening to others, considering well what they say,\n		Pausing, searching, receiving, contemplating,\n		Gently, but with undeniable will, divesting myself of the holds that would hold me.\n		I inhale great draughts of space,\n		The east and the west are mine, and the north and the south are mine.\n\n"
		 "		I am larger, better than I thought,\n		I did not know I held so much goodness.\n\n"
		 "		All seems beautiful to me,\n		I can repeat over to men and women You have done such good to me I would do the same to you,\n		I will recruit for myself and you as I go,\n		I will scatter myself among men and women as I go,\n		I will toss a new gladness and roughness among them,\n		Whoever denies me it shall not trouble me,\n		Whoever accepts me he or she shall be blessed and shall bless me.\n\n"
		 "		6\n		Now if a thousand perfect men were to appear it would not amaze me,\n		Now if a thousand beautiful forms of women appear'd it would not astonish me.\n\n"
		 "		Now I see the secret of the making of the best persons,\n		It is to grow in the open air and to eat and sleep with the earth.\n\n"
		 "		Here a great personal deed has room,\n		(Such a deed seizes upon the hearts of the whole race of men,\n			Its effusion of strength and will overwhelms law and mocks all authority and all argument against it.)\n\n"
		 "		Here is the test of wisdom,\n		Wisdom is not finally tested in schools,\n		Wisdom cannot be pass'd from one having it to another not having it,\n		Wisdom is of the soul, is not susceptible of proof, is its own proof,\n		Applies to all stages and objects and qualities and is content,\n		Is the certainty of the reality and immortality of things, and the excellence of things;\n	Something there is in the float of the sight of things that provokes it out of the soul.\n\n"
		 "		Now I re - examine philosophies and religions,\n		They may prove well in lecture - rooms, yet not prove at all under the spacious clouds and along the landscape and flowing currents.\n\n"
		 "		Here is realization,\n		Here is a man tallied-he realizes here what he has in him,\n		The past, the future, majesty, love-if they are vacant of you, you are vacant of them.\n\n"
		 "		Only the kernel of every object nourishes;\n	Where is he who tears off the husks for you and me ?\n		Where is he that undoes stratagems and envelopes for you and me ?\n\n"
		 "		Here is adhesiveness, it is not previously fashion'd, it is apropos;\n	Do you know what it is as you pass to be loved by strangers ?\n		Do you know the talk of those turning eye - balls ?\n\n"
		 "		7\n		Here is the efflux of the soul,\n		The efflux of the soul comes from within through embower'd gates, ever provoking questions,\n		These yearnings why are they ? these thoughts in the darkness why are they ?\n		Why are there men and women that while they are nigh me the sunlight expands my blood ?\n		Why when they leave me do my pennants of joy sink flat and lank ?\n		Why are there trees I never walk under but large and melodious thoughts descend upon me ?\n		(I think they hang there winter and summer on those trees and always drop fruit as I pass;)\n		What is it I interchange so suddenly with strangers ?\n		What with some driver as I ride on the seat by his side ?\n		What with some fisherman drawing his seine by the shore as I walk by and pause ?\n		What gives me to be free to a woman's and man's good - will ? what gives them to be free to mine ?\n\n"
		 "		8\n		The efflux of the soul is happiness, here is happiness,\n		I think it pervades the open air, waiting at all times,\n		Now it flows unto us, we are rightly charged.\n\n"
		 "		Here rises the fluid and attaching character,\n		The fluid and attaching character is the freshness and sweetness of man and woman,\n		(The herbs of the morning sprout no fresher and sweeter every day out of the roots of themselves, than it sprouts fresh and sweet continually out of itself.)\n\n"
		 "		Toward the fluid and attaching character exudes the sweat of the love of young and old,\n		From it falls distill'd the charm that mocks beauty and attainments,\n		Toward it heaves the shuddering longing ache of contact.\n\n"
		 "		9\n		Allons!whoever you are come travel with me!\n		Traveling with me you find what never tires.\n\n"
		 "		The earth never tires,\n		The earth is rude, silent, incomprehensible at first, Nature is rude and incomprehensible at first,\n		Be not discouraged, keep on, there are divine things well envelop'd,\n		I swear to you there are divine things more beautiful than words can tell.\n\n"
		 "		Allons!we must not stop here,\n		However sweet these laid - up stores, however convenient this dwelling we cannot remain here,\n		However shelter'd this port and however calm these waters we must not anchor here,\n		However welcome the hospitality that surrounds us we are permitted to receive it but a little while.\n\n"
		 "		10\n		Allons!the inducements shall be greater,\n		We will sail pathless and wild seas,\n		We will go where winds blow, waves dash, and the Yankee clipper speeds by under full sail.\n\n"
		 "		Allons!with power, liberty, the earth, the elements,\n		Health, defiance, gayety, self - esteem, curiosity;\n	Allons!from all formules!\n		From your formules, O bat - eyed and materialistic priests.\n\n"
		 "		The stale cadaver blocks up the passage-the burial waits no longer.\n\n"
		 "		Allons!yet take warning!\n		He traveling with me needs the best blood, thews, endurance,\n		None may come to the trial till he or she bring courage and health,\n		Come not here if you have already spent the best of yourself,\n		Only those may come who come in sweet and determin'd bodies,\n		No diseas'd person, no rum - drinker or venereal taint is permitted here.\n\n"
		 "		(I and mine do not convince by arguments, similes, rhymes,\n			We convince by our presence.)\n\n"
		 "		11\n		Listen!I will be honest with you,\n		I do not offer the old smooth prizes, but offer rough new prizes,\n		These are the days that must happen to you :\n	You shall not heap up what is call'd riches,\n		You shall scatter with lavish hand all that you earn or achieve,\n		You but arrive at the city to which you were destin'd, you hardly settle yourself to satisfaction before you are call'd by an irresistible call to depart,\n		You shall be treated to the ironical smiles and mockings of those who remain behind you,\n		What beckonings of love you receive you shall only answer with passionate kisses of parting,\n		You shall not allow the hold of those who spread their reach'd hands toward you.\n\n"
		 "		12\n		Allons!after the great Companions, and to belong to them!\n		They too are on the road-they are the swift and majestic men-they are the greatest women,\n	Enjoyers of calms of seas and storms of seas,\n		Sailors of many a ship, walkers of many a mile of land,\n		Habituès of many distant countries, habituès of far - distant dwellings,\n		Trusters of men and women, observers of cities, solitary toilers,\n		Pausers and contemplators of tufts, blossoms, shells of the shore,\n		Dancers at wedding - dances, kissers of brides, tender helpers of children, bearers of children,\n		Soldiers of revolts, standers by gaping graves, lowerers - down of coffins,\n		Journeyers over consecutive seasons, over the years, the curious years each emerging from that which preceded it,\n		Journeyers as with companions, namely their own diverse phases,\n		Forth - steppers from the latent unrealized baby - days,\n		Journeyers gayly with their own youth, journeyers with their bearded and well - grain'd manhood,\n		Journeyers with their womanhood, ample, unsurpass'd, content,\n		Journeyers with their own sublime old age of manhood or womanhood,\n		Old age, calm, expanded, broad with the haughty breadth of the universe,\n		Old age, flowing free with the delicious near - by freedom of death.\n\n"
		 "		13\n		Allons!to that which is endless as it was beginningless,\n		To undergo much, tramps of days, rests of nights,\n		To merge all in the travel they tend to, and the days and nights they tend to,\n		Again to merge them in the start of superior journeys,\n		To see nothing anywhere but what you may reach it and pass it,\n		To conceive no time, however distant, but what you may reach it and pass it,\n		To look up or down no road but it stretches and waits for you, however long but it stretches and waits for you,\n		To see no being, not God's or any, but you also go thither,\n		To see no possession but you may possess it, enjoying all without labor or purchase, abstracting the feast yet not abstracting one particle of it,\n		To take the best of the farmer's farm and the rich man's elegant villa, and the chaste blessings of the well - married couple, and the fruits of orchards and flowers of gardens,\n		To take to your use out of the compact cities as you pass through,\n		To carry buildings and streets with you afterward wherever you go,\n		To gather the minds of men out of their brains as you encounter them, to gather the love out of their hearts,\n		To take your lovers on the road with you, for all that you leave them behind you,\n		To know the universe itself as a road, as many roads, as roads for traveling souls.\n\n"
		 "		All parts away for the progress of souls,\n		All religion, all solid things, arts, governments-all that was or is apparent upon this globe or any globe, falls into niches and corners before the procession of souls along the grand roads of the universe.\n\n"
		 "		Of the progress of the souls of men and women along the grand roads of the universe, all other progress is the needed emblem and sustenance.\n\n"
		 "		Forever alive, forever forward,\n		Stately, solemn, sad, withdrawn, baffled, mad, turbulent, feeble, dissatisfied,\n		Desperate, proud, fond, sick, accepted by men, rejected by men,\n		They go!they go!I know that they go, but I know not where they go,\n		But I know that they go toward the best-toward something great.\n\n"
		 "		Whoever you are, come forth!or man or woman come forth!\n		You must not stay sleeping and dallying there in the house, though you built it, or though it has been built for you.\n\n"
		 "		Out of the dark confinement!out from behind the screen!\n		It is useless to protest, I know all and expose it.\n\n"
		 "		Behold through you as bad as the rest,\n		Through the laughter, dancing, dining, supping, of people,\n		Inside of dresses and ornaments, inside of those wash'd and trimm'd faces,\n		Behold a secret silent loathing and despair.\n\n"
		 "		No husband, no wife, no friend, trusted to hear the confession,\n		Another self, a duplicate of every one, skulking and hiding it goes,\n		Formless and wordless through the streets of the cities, polite and bland in the parlors,\n		In the cars of railroads, in steamboats, in the public assembly,\n		Home to the houses of men and women, at the table, in the bedroom, everywhere,\n		Smartly attired, countenance smiling, form upright, death under the breast - bones, hell under the skull - bones,\n		Under the broadcloth and gloves, under the ribbons and artificial flowers,\n		Keeping fair with the customs, speaking not a syllable of itself,\n		Speaking of any thing else but never of itself.\n\n"
		 "		14\n		Allons!through struggles and wars!\n		The goal that was named cannot be countermanded.\n\n"
		 "		Have the past struggles succeeded ?\n		What has succeeded ? yourself ? your nation ? Nature ?\n		Now understand me well-it is provided in the essence of things that from any fruition of success, no matter what, shall come forth something to make a greater struggle necessary.\n\n"
		 "		My call is the call of battle, I nourish active rebellion,\n		He going with me must go well arm'd,\n		He going with me goes often with spare diet, poverty, angry enemies, desertions.\n\n"
		 "		15\n		Allons!the road is before us!\n		It is safe-I have tried it-my own feet have tried it well-be not detain'd!\n\n"
		 "		Let the paper remain on the desk unwritten, and the book on the shelf unopen'd!\n		Let the tools remain in the workshop!let the money remain unearn'd!\n		Let the school stand!mind not the cry of the teacher!\n		Let the preacher preach in his pulpit!let the lawyer plead in the court, and the judge expound the law.\n\n"
		 "		Camerado, I give you my hand!\n		I give you my love more precious than money,\n		I give you myself before preaching or law;\n	Will you give me yourself ? will you come travel with me ?\n		Shall we stick by each other as long as we live ? ";
	return poems;
}


// Finds the length of a poem given by `poem_name`
// Args:
//   poem_name: Pointer to c-string of the poem name
// Returns: poem length as an int
extern "C" int get_poem_len(const char **poem_name) {
  // initializePoetry is called every time becuase the main process will only
  // call these functions twice before exiting. This is simpler than requireing
  // the program to initialize the map first.
  return initializePoetry()[*poem_name].length();
}

// Fills `poem_buffer` with the poem specified by `poem_name`
// Args:
//   poem_name: Pointer to a c-string of the poem name
//   poem_buffer: Pointer to a buffer to be filled
//                NOTE: Use get_poem_len() to size the buffer correctly
extern "C" void get_poem(const char **poem_name, char* poem_buffer) {
  // See get_poem_len comment about initializePoetry
  string poem = initializePoetry()[*poem_name];
  poem.copy(poem_buffer, poem.length(), 0);
}
