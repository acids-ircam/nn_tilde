{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 9,
			"minor" : 0,
			"revision" : 0,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 425.0, 216.0, 1019.0, 690.0 ],
		"gridsize" : [ 15.0, 15.0 ],
		"toolbars_unpinned_last_save" : 2,
		"showrootpatcherontab" : 0,
		"showontab" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-13",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 250.0, 87.0, 74.0, 22.0 ],
					"text" : "print coucou"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 350.0, 147.0, 115.0, 22.0 ],
					"text" : "string.compare prior"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 350.0, 251.0, 51.0, 22.0 ],
					"text" : "pcontrol"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 350.0, 216.0, 61.0, 22.0 ],
					"text" : "enable $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 78.0, 263.0, 51.0, 22.0 ],
					"text" : "pcontrol"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 78.0, 223.0, 61.0, 22.0 ],
					"text" : "enable $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 78.0, 147.0, 115.0, 22.0 ],
					"text" : "string.compare rave"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 78.0, 70.0, 69.0, 22.0 ],
					"text" : "r rave_help"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 630.0, 483.5, 24.0, 24.0 ],
					"svg" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 9,
							"minor" : 0,
							"revision" : 0,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "box",
						"rect" : [ 0.0, 26.0, 1019.0, 664.0 ],
						"gridsize" : [ 15.0, 15.0 ],
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-25",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 573.0, 335.0, 28.0, 20.0 ],
									"text" : "-5"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-24",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 573.0, 239.0, 28.0, 20.0 ],
									"text" : "5"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-10",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 680.0, 245.0, 71.0, 22.0 ],
									"text" : "s rave_help"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-12",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 680.0, 197.0, 33.0, 22.0 ],
									"text" : "prior"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 680.0, 147.0, 40.0, 22.0 ],
									"text" : "active"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"hidden" : 1,
									"id" : "obj-9",
									"index" : 1,
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 680.0, 92.0, 30.0, 30.0 ]
								}

							}
, 							{
								"box" : 								{
									"autofit" : 1,
									"data" : [ 34467, "png", "IBkSG0fBZn....PCIgDQRA..BXN..D.hHX....PCOVB4....DLmPIQEBHf.B7g.YHB..f.PRDEDU3wI6cmGdUUcu2.+6JmLOGlSXJ.BU.wAFTApHzVopUoZqP6q1aq9p1Vakq1dauV8ZEeqcv5LspUpzK2ps0ZPsE05bM8JNPsQ.gvTfj.jDRNj44SNC466erOACgLbNmrO60dexuOOO77PNm8Yu9sGOqyZuV+V.BgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBGCktC.gPHDBgvgx0V25Vyb5Se5iOu7xax.X5.XV.XL.3tTJ0Qza3IDBgPHDBQLJRNJRd8jbWbvUjtiUgPHDBgPHh4TXgEFOIergnx38lacGyBgPHDBgPDSoolZJGRVRXTobRxMq63VHDBgPHDhXF974aEjLPXVobRxOiticgPHDBgPHb7B10U9MQPExII8RxDz81fPHDBgPHDNZc1Ym4SxphvJkSRdE5daPHDBgPHDBGKRFGIu4gQExII2FjzQsPHDBgPHDQllZpoQQxsOLqTtWRlot2VDBgPHDBgvwgjJR9kFlUHuGKP2aOBgPHDBgP33PxzI4aXRUJ+ao6sGgPHDBgPHbTnQqjeQznqmXF90jT5W4BgPHDBgPDpHYxjbylTExIIeURFmt2tDBgPHDBgvwfjKfjsYhUJe6jLdcucIDBgPHDBgi.Iimj+JSrB4jjkQxj081lPHDBgPHDNBjLORdXStR4tIY55daSHDBgPHDBaOZL.OuZStB4jjsPxr081mPHDBgPHD1dzX.d9RQgJk2QKszxXz81mPHDBgPHD1djbFjr9nPkx8d7ie7b081mPHDBgPHD1Zznqqb8QgJjSR5sxJqbx5daTHDBgPHDBaMRlHI2RTpR4Apt5pyW2aiBgPHDBgPXqQirth6nTkx4wO9wmot2FEBgPHDBgvVijWZzpB4jj0UWcyQ2aiBgPHDBgPXaQRWz7mvfNIM0TSKP2amBgPHDBgPXaQxzHYQQyJk2Zqs9Yz81oPHDBgPHD1VzHUH1RzrR4s2d6Wkt2NEibEutC.gPHzER5B.YCfo.fYBfY.fdl7P1K.JPoTspovSHD8BI+R.34ilkgOe99dokVZOWzrLDBgPHDFTs0VaSfjeUR9VgPim0FISW2AsPLRFIiij2WzqMxOgeE.T5d6UHDBgHl1S9jO43KojRtk.ABDIOB72GxWVKDZQwEWbhLz9QzCWuC.hS2auBgPHDwjnQqrsTRtqg62X62u+0n6sGgXjllat4QSxpFtW+FBZr7xKOYcu8JDBgPDShj4RxxLwu3NPQEUTp5d6RHFoviGOygjdMwqgGPM2byyR2auBgPHDwjHYFjLfY+k2ABD3Iz81lPLRfOe9tby952A455GT2auBgPHDwrH48Ds9R7Zpoloq6sOgHFlhj2Vz5529ya9luYV5diVH5MY.MIDhXFjLN.3A.IDkJh8AfyPoTcGkV+BwHRAu182Afq0BK1tUJkKKr7DhgjLBjEBQLi1au8wgnWkxA.lM.thn35WHFwgjI.f2BVakxA.1sEWdBwPRlfgDBQLizRKsy1BJlMSxLTJUGVPYIDwzHYp.nHX7idsZ+BMTlBwfRZwbgPDKY0VPYDG.90VP4HDwzH4nAP4PCUJuiN537UJ0yZ0kqPHDBwHEJZQoWsfzQK7IDwDd0W8UmuEe85Izd6s+kz81uPHDBQLsBKrvIXwe+9gKnfBjANlPDdR7e7O9G+jt6taK9x0S32n6c.BgPHDw75ryNuNM7k7eKcucKDNEj7J0v0n81gIoL15D1ZR5RTHDwBTjrF.LNqtfat4lGU1YmciVc4JDNMjzMzv0n8xjTJUUQyBfjiC.0qTp.QyxQD6RF7mBgvwysa2SGZ5K7yJqrdFHMxgPLnJnfBRD5sR4e6nckxC5o.vMYAkiPHDBg8DIeIc97w8506R089.gvNqyN67Fz3knEQiIvnnNR1HIOrUTVBgPHD1N0TSMiWiegeOZonhJJZNwFIDNVG+3GOWcdwYIkTxXspsUJULWHDBwHYj7OqyuzuW9Y5degPX2Td4kmLIqRWWT1YmcdiV41KIeMRdyVYYJDBgPXKzPCMLEc8E9CfIq68IBgcQwEWbhjbuZ75w8CKdrzQxwQxPMMppHo7j1DBgPDSPQxsowuzu+rKZQ8kUgvNijIQxh04Eic1Ym4q68CCjRJojjnwXioJ4dFBgPHb75niNVpN+R+AgLqBJFQijIRiejpNcuV31qhFyDvCU1YRUTQEkU80W+WkjszSf1UWccNVQbJDBgPDUTXgEFOMFjU1QAHY55dejPnCETPAtH46q4qA6fjIZUayj75CVt+M1qIvHRFGIuPR9mHY8CR79mspXUX+I4dWgP33Px+S.7K0cbLHdckRcI.f5NPDBqBM5RFaF.59oF84TJ0e2JJHRlL.5rWuTo0We8WdYkUVaKZQK5uAf4EJqmCdvCl7rl0r5JpDjVDRp17l2bbyYNywUN4jS7olZpI5xkqDxHiLRD.I.fDAP7.nm9fe2c0UWd6niN5pyN6rC.zw68duWWqYMqYD8jyjTwbgP3nzRKsLlLxHiZ0cbLTBDHvsFe7wudcGGBgUgj2G.9gZNL9mJkZwvj+QwjToTpSYcRxqC.+2lPQbIJk50Lg0iYSUbwEmvDlvDRNkTRIyTSM0QCiIJpI.fICfY.fOE.lM.FkIUl9.v6.feO.dFkR42jVuBgPHLYJR9NQim8czPmc14M.oAPDi.zUWcc084zeuj7vV80bs0VaSvr21H4XH4y22WOX21oCSJzqBZ3dEjzka2tSmjSjjKfjeIRdmj7uPxpMossgika06SDBgPDhH4mU2eKQ3JPf.E41s6uKIuZRNaF5oRMgvQvsa2mUeNseyjL4N5niulEe41Ovr21BV46CSxGquuGI+blYvWYkUtDyN9CFmwQxzIY9jb4j76Rx+.IcalwezPQEUzLhF6SDBgPLLQizulY05T5T.RdKjLIcuOUHFtN5QO5nnQqi2iakjphJpnDHYaV30UGlQgezKI+AAW+WZed83n4OwIUeAETPjNnUiam6bmo0QGcLYRtXZLfT+sTCO0Byz5V25RsW6yM8mFhcj7HVEBgi.MZwpuitiCSTmvXPp895NPDhHQgEVX7Ke4KuD.LM..+98upDRHgWB.fjOD.9dVX3LckRUtYtBOxQNRNSYJSoA.iw1RVYkU887dczQGWXJojx+vLKuf1Qas012nxJq7vABDnKOd7vLyLy3750aBwGe7ImUVYkQlYl4XRLwDmb7wG+mB.KD.KC.iIJDKZWKszxrxJqrNzN24Nm4YcVm0A.vBUJ0Go63RHDhQz73wyrzbC2DMcajTZjDgSihFoAPRR1QGcbhtgAIOOK9ZnebzXCjFc2CRxZwI2PltHYsV71XTke+9YCMzv.99c0UW7UdkWgqcsqkKZQKh.fe8u9WeP+LQIApppphI+QH8PlsoDBgsFIckTRI8F5NNhhtW.bM5NHDhvAI+F.3+C.vgNzgVPpol56G70yF.a0BCki.fetYuRO3AO3jwmbc4u.8JKuzd6s+cPLTKT62ueb228ciy8bOWzUW8eFart5pCaaaaCYkUV3Zu1qEu8a+13QezGE4jSNVbzh3xKu7189129xvpKXgPHD.fjeKqtIYzgppppTG58FBg9Qxo0y4sEUTQyqWud7jrDK9RmoDE19TABD3s6o.NxQNxb648d+2+8mnEu8E0UYkUR.vUrhUPOd7n6vIT0Q6s29U0RKsLaRtHRlfYedfPHDh9nwFaLace2eKzmQ26uEhgRwEWbhLX17nt5pa185sTzHarXkt0nw13wN1wlSuKjst0slC.v5W+5Shl+.9zVnlZpgs0lUNVcMccPx4GMNevpI8qQgPXaQx+B.tBcGGVjGUoTqU2AgPLXBDHvFiKt3tN.LakRUROuNIWG.taKLT1M.NakR0sIudUjbOvXByoGSc+6e+Md5m9ouU.bVlb40u1291G18t2MhO93wjlzjvjlzjPd4kmUTzNcWqRo985NHDBgHlCImmVa+Eqmse1LULx19129VHMRMh426WmjWiUewRyM27nM6sORlAIuIqdaou74yGm27lGgQ+Zm.fuvK7B5NrbRtQy9bCqjzh4Bgv1gjwAfx.vT0crXwRPl9oE1QASMhGB.mmRob2yq60q2KHgDR3cr3vY0Jk54FtqjBKrv3W7hW7zRJojtb.7e..aSSRWYkUhm+4edzRKsfq7JuRbFmwYn6PxoYUJk5kzcPDIjJlKDBaGRtF.7r5NNzfbTJUS5NHDh9hjmE.JSoTs1yq0ZqsN2zSO8hs3P4UTJ0kgdkkTBCplZpobxJqrVB.tI.boC0GP3nc5Jk5.5NHBWREyihH4rfwDrPdvXxD43vH0NcD.TA.b2TSM0TokVZ6KbgKzOhrazHDwTHYx.nE.LRbT1eZJkpTcGDBwPozRKcJSe5SubXsoc4tN1wN1nl3DmXGg5GnjRJIoYNyY9o.vpAvs..IM6MxQq.XBJkJjOewNHdcG.wXTEUTQwWe80myV25VWI.98XHtoU1YmMVvBV.HI.vA.vNAv9CDHvQZngFpo1ZqsgxKu7VppppZuyN6rqcu6c6ykKWAN2y8b695u9qGv3XXhM1XiIWas05ZRSZRdSM0TaA.spTJoh9Bmn+KLxrR4..iC.REyE1Z+y+4+bzSe5SuDXwyEJczQGWXHTobUKszxnyHiLVN.99.XwQ+HSXSkA.ddRdoNo5CIsXdDfjSE.O..VB.FKrmUhnK.7S.vioTpl0cvHDgBRNZ.TmtiCM5qpTpQhcgGgCQwEWb5yctysbXwSvNc2c2OoKWt9l826EbLoLM.bU.3GAfrsxXSX6sHkRUjtChPkLyeFYdU.bUtc69sJojR9r0We8SB.YBfTN7gObJaYKaIym4YdlId3Ce3KE.GD.34dtm6eSoTiQoTIpTp3T8BLNNDO.xp81a+RgQ2dAvnkyVB.9+EAwXR.3mAflH4+IIkmNhvIXi5N.zrQZC1UgCRQEUTByctyc6v5m0Kace6ae2bueARFGImEIuO.3A.GBFyhtgbkx6pqtvK+xuLtm64dvu6286P6s2t4F0B6hmR2AP3PZw7H.I8FHPfuV7wGeAgvhGGIuHkR85g55+G9C+g48K+k+xpZokVFU1Ymci..M1Ximc1Ym8Nh3f1n+suBkRs2AagH4jgwM1pDFcGlnVFhfjw0d6sOt8u+82xBW3BcT8ALg4ijyD.kLjKXrsmPoT2jtCBgnenH4KBfKypK35pqtycricr+qd9aRNQ.7ygQCh0I.pG.tAPU.nF.TqGOdZp0Vas85pqNu986u6wO9wmznG8nWlKWtNwSj5sdq2BWzEcQ..X7ie73i+3OFie7i2B2xDVn3UJU.cGDgBoh4Q.RVF.VbuSYTQgx3dZqs1dzLxHiSTFM1Xi4mc1YueXzZ3QpG+fG7fe+YMqY00fT1ICfKA.OH.FS2c2cg0TSM+yxKu78r28t2ibvCdvie+2+82DLZkhHdxcHXq3uS.LW.7d.3KHc6lQlHoB.eL.l2Psrw3JToTxL.pv1gj+m.3WZ0ka2c28y4xkqUaFqqBJn.Wqd0q9DM1TSM0DdwW7EQf.Av4e9mOl8rm8f8wENTd858qjTRIEJMjpvohjepH3ybN6bm6Lsd96BKrv3I4ESiIyfafje1JpnhYtwMtwLJrvBi2qWuKlj0uicriI1m0SljbKkUVYqY8qe8yo3hKdUj7ODBIb+1H4ySx6fjWdv9j2PEypVZokwRx0Px2d.VuEEbaHhZlAR5hj+ffqqAs07EwtH4RCgygGInJcerPH5KRNaMdMgojEUV+5WeRae6aOT9tRQLl1ZqsGxLNGRDCokVZYL8bBRf.A90s1ZqeNR9tgv4SESxuQnTFznBteJZTw6p6YE7AevGbsabiazTtwFM5SeItoMsorevG7Au99Dq+jgy51sa2oSRo4JFAJ34ttGV24MFhtOdHD8EIOrNtVvue+W+vM1apolxgjeaZLikJF45rLiqErBRWYYXpnhJJ0ErfErRkR8W68qSiGM+x.vlfwnEu21C.1ALROhGF.UCfZangFZrs1Zqst5pqNl0rlkWLLxq4znahnTJkuHccDBkwjgw11Ahzt0CIyQoTM1mWak.3BTJ0O1DBSgMGI+R.340cbXi3RoTQbWDSHLSG9vGN2oN0odLMTzspTpb.PX2ufC98xeA.bO.HreB2hXRcCfopTpJ0cfHhhHohjuOIquOudbjbiA+UZt20t10pu0a8VyE.IB4GCo.fqu427alUM0TyWijdapolxo2KPwEWbhjbS88CVZokNdZzsZtaRt5BJn.WVVTKhJHYBjziE1pINA1wzupXDHZ7cbulNtHngFZ3xC23szRKMKR9azQ7JbD7TSM0Hit2XY7S5Wr6pWuVbj7UC95EWTQEMh7KYI47I4uijkPxNHYfA4hk8hgH0cRxo1OeteqEs4HhRH4ZMga1FS4nG8non6iKBA.fWudu.cccPwEWbh8SHo3.7CWI4WRWwpvQoERldT9RGglnnQ+tyMIOMfSz5B+tdcBfo7HzHYNj7JapollgSnUhI44GAWr7yFp069129xOPf.8t0U8B4IP3XQxTinaqFiq1ZqUlxvEZGISgFMphVf9bu8fOI0szeM1EIWfthSgiz6PitarHVBMlXClcedsKrWG3qGlTkFau81mTuVuAH42k1nILHRN1d8mJRdzH4Jklat4qECw9rm4Ydlkz6OSKszRDMfQI4nH4vIsSJFlH48FImmDqq4ladz59XiXjMZzHSujNuN3we7GeU.v0q7JuRRjbEjrdZjxF6u38xzYrJbjlrEeYkHZijaxiGOmTkB862+960AcSaRB4odpmJs94jJ2s1Zqiqehq6jjUQxkOXqyppppToQVbYYz3lZWYv+cYj7yPxyskVZY1UVYkSt3hKdTEWbwou90u9jfwLTpqf+KocsqccljjG3.GXh..ETPASa3bkRf.Ad61au8qhF8k76kjuAI+h8JzSs6t69j1OTd4kmb3tOkjWRvO+OijoMzeBgYJXlRPzOZqs1xU2GeDirQxq1JNWurxJiUWc08660c2cy9budxA3d0znKj9FQ4vUDaYwV80UhHje+9WGMxK3CVeCOgfGXCTVYkcaG9vG9y3ymuGo2Gwas0VMsAXve+u+2WV+cVUf.ApcCaXCmTbRxqsWKR+FCL5zW7bSxQSxsDEV2mXVA8ke4Wd58y6+gEVXgg8SPfF4e8dbsjz12MghUPxeeT37jXE4q6iOhQtH4jshSxqolZ30bMWC85Mjyhgqavh6Vas0wEMiWQrkN6ryYXUWSILAG9vGNWRNpA58OxQNxP0e1ZDlT2Xwsa2yfCdVq3V66mo3hKdBzHelewCz5kjw4ymuqvjNGOp68e+2ehG8nGMERtyAZQ56ORITPxoPxsGbcTFImd3tNDgm1au87rhyYbvNcceLRLxDMxRR8eSXahZs0V40bMWCeoW5T6sLM1XirwFa7Tdc2tcOnCXOZzp4BQnx1zcfEl.R9aGhC3+bSpbVxPTNjF+HfHVWc007F9meaar2hKt3vdzVSRUas0Vu6ihqyILHacp3mj0hD8uyV2GiDi7Pi9U9lrhSverG6w3JW4JYas01I856e+6mKZQKhUTQEmxGY.h4QSxqhjq1qWueKqH1EwDdBq95KQDfF2T5aQxkLDKWbLXp+6se62lUVYk82A8kGEhs3c61c5c1Ym4SxURx+SRd+zDlMqH4WM5ecfkokJpnhYFI6GJqrxlJ+jz5XIs0VaSX3tuUbxZpollgFO2vo3708wIwHOj7hspSv2xV1BO1wN1I8Z6YO6gie7imqXEqf986+jduicriM09DqIwSNCnIDgiIpqqyDgAR9sau81uxPX4lHIYSM0Dm27lGesW6Tm6EhzLEhtPiJ9+mowj8yL.fxiGOyxJuJwr40q2uKIGzbi9.ruX7jr2Ch2uDkzpjYQQxsoiyGbXVttOPIFY4nG8niRmmvWWc0wktzkR.vMu4MeJuO507bQvua5T6qKBQn4OouqzDgLRlII4C9fO3PNwdvdMZ0+m+y+IqolZNki5s0VaWpUD2lIZ7j.NwuhrnhJJWK5hjno82TSME18YbZru3p4mz54C0fAVDBZs0Vik51TQSedcerRLhRbjbWQ6SpCDHv1dm24ctee97cRudWc0EW6ZWKA.WzhVDat4lOo2ugFZnMZz3QISiY8YgHR4kRVXyYfFCTxPsh4gxzSbad858bcx8SYRFOI+SQuqOrTuQmc1Y9Qv9fjI4sEbcTFIk7KcjSwS9IQHFXWgtOXIF4vue++3n8Izd858sfQJ1My5qu9S5813F2HA.A.25V25.sJpMZGihQDVlduZSDxH4iPRt28t2AM+ASRWAO31Q0UW8JBgSB7xgnOq2m0uhjmCIuNRtJZRsRKImWjttJojRRpyN6bFj7KRiA85fkgXr698jLRx44YPxeEMZA8EFI6GGoq4laNRlMXGo5qn6iWhQFpu95maz9jYud8tcXL+W..jjGOexWgr28t2STo7m3IdhncnHFY61z2UZhvFIUG5PGZbbHxi0zn+GylZpoUF7uGJ0Sxq0R1HFDAiim1jVWw40qWm7zebGjbAQx1ds0Vadj7CI40SoemGxnQWCJhlMXGg5eS2GyDw9Ju7xSljsDMOQNPf.tW25V2IZLjG4Qdjw2y60c2cy0t10xktzkx268dunYXHD2GLoTXsUvwDn1Aj7BAveUoTi13OY282x0TSMcckVZo+wEtvE5yZiv9GIud.LOkRcJ477fueb.Hoie7i6pgFZPEHP.1RKsDHyLyLPs0Va20VasbricrJ.DuKWtxXFyXFe57xKuWvue+nqt5BoklirKa8CUJ0CBiVqIbnpt5p+BSXBS3SCfaWoTg6meDmVas0OS5om9eW2wgCxMpTpMp6fPDSSQxB.vUEMKjW7EewI7E+heQ2872kWd42Z94m+CC.31sa7Vu0agq3JtBm52gHb.BDHv2zkKWaT9tZGNRNyVZokw1Ou9swfs.dwEWbh88mjEHP.OaYKa4zr7.dXhjShjqijs02soASiM1HKnfBFr9Enc2KwHrK9rssssLI4syHHyuLRRAETfKJ8Ozv02U2G2Dw174y2ph1mDu28t2Kp2k4i9nOZ57SFP8riN5HZGBhQ11ejL1xrCjVLueTTQEkvBVvBJE.qRoT6rmWmjuVc0U2pG6XGaq0UWcSZzidzUzqOluO3C9fosjkrjpr9H1znH4XAvhAv+W.rpHcEcvCdP7ge3Ghq4ZtFSK3hRN..VfRoZOB9rwQiAp2eQ9038OOd7b4IkTRuntiCGl+CkR8P5NHDwlZpolFUVYkU8QyxnyN6b8olZp89IzFmOe9d63iO9KLZVtwn1cWc00d8506wRHgD5LgDRvqKWt.L529ICfD.P2.fd85MfWud84ymOuIlXhdiO936hjdAf+f+qGwmbxImD.RE.4.fQCf7.vrAvIky3cfp..WiRodWD9OQbgcla2tm.IKo2uFI+c85+ei84WmMOqOJitHoq1ZqsbI4kSxmlFCl0PRf.A32467c3QNxQLie4azlaRlUDtORwHX.kNRPvVKOp1GViQICRIQTAMFuG6LJe9aYEVXgwC.TSM0L98su8s.+98+9Q4xLVRQMzPCWPgEVX5PCMdJIUEVXgwSxbH4oWVYk8u42u+2Py6SBE+CRd1TF+Ww1H4mtO+cpA+uwwdMAG30q2aVCgmNnZpolxgjKijOHGhJcczidTVUUUE8tLzb0HIGit2AGKoqt5ZM59fZOJszR0cHDNtScerSDahj+fn8IuEUTQ4Bbht6oSNKdY07TWc0c9vl1SFHYR9746yRR6zWp2AI+9jbT5d+iPy74y2U0qSLND50rR1HLpxKu7r84y2mgjah8p+C5P0QEUTwjz8N0XAEUTQIPiaZpcG6XGqs29se66S2wQX3t08wOQrGRNin8ItM0TSeqdJuFZnguQzt7hgT+N1wNxVmmeDpnwSc4yP8kosBPxGxiGOyjxX7RPRUYkU1Y0mSRVrtiK6BRF2QO5Qmne+9ud5bmLYB7lu4adN5deoSme+9+F59.YOV+5W+J+m+y+4UOzKoswOS2G+DwVHYBznK6E0DHPfsiOoQpNomp7fo3hKN89KYJLRxt10tB6YnZa.EImIIeLKXWTUj7+fjSmRkwE8fjeU1OYsDm7r6Yz1q+5udZMzPCKijOuEbgqo5sdq25JCmsURlHiv9odrlfeIaHOdDhlppppNL.ha26d22jtikvv8p6ighXKj72DsOo8sdq2Z78Tdae6auuMf0.4u..TbwEOpnZvYi0Ymc9t56LCyQvwSzoSx6lFyaJCGtI4VH42kFS3hRtzT7IH4Dqqt5lzANvAl3fbRjsr+fY2rgMrgDpnhJNq.ABzyLnosWUUU08Eh+vq33mjR.+Oh56Ls4762eeGXzZy5W+5W..vgNzgVmtikvfTwbgogjmWz9D1JqrxueuJRUf.AdmP7iN4fw3OKBJ1NnwD91lH4cPxuFIWIIWPmc14o41s6ITd4kmcQEUTpEWbwIFb.oFGMZwUUu9WbEUTQo51s6uQDDCCaG5PG5KY8mUD8PRUs0VaFjbAj7lI4aw9+67eLRtflZpobJpnhR.1z9VuvBwgnB0jb4gxEUd734pnLnACKETPAt18t28r84y2CM.WvZmreRl+fs8TUUUMld+AZs0VGmEsqz14UdkWIIZSNlVQEUrcD7l8tc6Np2hgln6SyGFQAETfK2tcmNIyljigFYngj4PL6HKrWHYJLJOVO750a0.3DmWricL3QNpL...H.jDQAQki4FhezeWvXLyA38K1iGO+pJqrxqaO6YOKqvBK7zdgW3EFcvYRznQ2YP0c2caR6UBcu4a9lyJJrsXqPirWVBjLMRlNIiW2wjvFhjW5f7dwyvazjewVYrGKofBJvUIkTxbBDHvSX52wybsxAZafjSgjrvBKL4foYyQroOQ2tcey58vzmXCaXCmdOwUKszxKo63ILb+Z33V5jbwj7g3P2WjKlj2IMdLyIZ0wpHzPiJCskn5Ypj7sdq2Z98pXiyue+gx3KJ.ClkyH4kQxMSitK5LnQWWPKsdJ0PVj4W+q+0iVGaqBgsCI24f7dqNbtvp4ladlVYrGqZCaXCITZoktn.ABD0+xjHT+lq529129XIIKnfBFQWIk0st0krtO.0iJpnh+QuiMud8VrtiovfkL4BEregddj7sGlw6lI4mBxih1VgFU3Mpps1Z6cQuNtWYkUFpikiKSi6ZFPjb6Qy8W8mm5odJoOTKDz3QpPD7wgQxb548BNv0BqG8W4kW9H1VHMZ4EewWL0FZnguHIKw7uUXDqC1+sFtKRxidziNQKeGkMBMF071BO7C+v42mXyVj5FCQQ0JlSimH30PyeexgI4E.oB5ZGIyhVPWJ64dtm6DMJUgEVX9g3G6sfM8bDZL3EsTaXCaHAcucKDZGIGOI41111tfMrgMj.I2EBNPPBDHvFCyqq1qt2dhkQR0t10tFuGOdtCZOxzG8aklH40SxwZ06erKnQeY0VnxJq7u0mvSo6XJL8fQwiSKk8S1kxjUBMZAcgdnH46FkOFSe97QDrB1268duYwPbV9st5pKSMu+Y.QxyM5tW6TAa5ORQHrTj7766EGc0UWaIPf.uUDbc00q6smQPbUc0UedjLTGw+QKinaY79CIucMeL4D9M+leSeO93R2wTXJZzGyUjzpmjk90TFjWVNRZI4r+t6taVXgEl9d1ydlICwTjmOe9r0YfDZL.msRMp6sYgvVfjlV5bqs1ZKWcu8LRzd26dGse+9ucpmL.RIAS8VmBR5Hl81LSjLUMbLneczidzB5a7Ym566gHSOqrPxerl1VJixzkskoolZxNmOvOQVRxNiFoeQqxeQ2auB6mQpyfRmmIsdZM8zSuFSZcIBCyYNyo93iO9ew+3e7ORps1Z6yAfRsvhelKaYKaas2d6mGIWBO4TH28Xgwgcwsp6.nGuvK7Bqsuu1nF0nbZCJ2tMyUVmc1Y9.3mXlqyvvz.PkjLOMU9ijnxJqrdEcGDCj5qu9UA.p63XnzbyM+Ssvh6uagkkPXeQya5h++T2aKhSPc7ie7YQRcjV71LIiuiN5XJjirlMXoMp0xqrxJ+88WLtksrkwq6XKL8yL4iQupt2fnw3CImgNZEQJ+98aIcgkHzin68OgpRJojjrv8Kmkt2dE1OiDawbE.LqAlzyZRqGwvGG23FWIJk5xO1wN1XAviZgk8UA.eojRJGA.X0qd0S1BKacy1LSm9TO0S886uWO8zSOcqNVFl7YVqnppppTAfcXdVHA.rCJ847nhlZpobb4x0eT2ww.olZp4+R2wPnZhSbhmgEUTcAfhsnxR3fLhqh4ETPAlUpIpK.TgIstDlnINwIVmRoV692+9yD.2kFBgOmFJSKW0UWcZPecQhSxQNxQ1vcbG2Q8826kQFYXayBDC.+l0JJu7xatl05xDLUXSNeIFiJqrx5U0cPLHtmbyM210cPDJH4mJ0TSsHqnrZrwF+iJkJfUTVBmkQbULO0TS0rRl+OtRoL09BpvbM6YO6VUJ08TSM0jNhR886VZoE7we7Gi.ANo6udyCzxSxKIZDG5vDlvD9g5NF5wy7LOysOPuWRIkjSavGZZsXN.VlIttLC2NImjtChXI986+Zf4MtohFdbcG.gAKq6oVTQE8AVUYIbVFwUw7oMsoMFyX83ymum2LVOhnubyM21UJ0c0XiMlM.9eLy0cpolJdlm4YvMdi2HN1wNVOu7Y89u+6mx.7Q1fcNO9Fpb61c5.Xc5NN..Ju7xeza+1u8ALsikZpoZJWyagLyVQ6yahqKyxF0c.DqnkVZYLtb45o0cbLH1G.bq6fHLXY4e+rxJqxspxR3rLhqh44kWdlR++srxJ6.87+I4YPiIsieqYrtEQGiZTipYkRccd73YF.3.C4GHDDe7wiezO5GgpppJbEWwUf8u+8C.f4Mu4cd..jbQjr2oHr0O5QO5ejYT15z3F23rM8s7srks7iGr2OszRyok24MyVL+SahqKyxmu81aWxRKCSjLtLxHi2P2wwPXMJkx1mIV5k1rpBZlybl0ZUkkPXqQxa1LFJ08dZzkjtH4BBNPqDN.jLNR9cMiyEHIqt5p4C7.O.OsS6zXM0TCYvYS15qu9ISxEzqxMdRdEZbSeXq5pqNMyZ+1vUYkU1PN802TSM8P5NNCSeay33DMtujckiIKcXWQxustOHNX762us3IpENH4iXg6hxW2auBgs.I+SlwUTXD3SaHVjGOdNcR5wLNmfzX1vqGG4HG4y41s6IPxCCGvDqQnhj2oYs+Z35du26MqgJd6pqt9K5NNCSlxrILIsxz9VXqnhJxrFH9i3Px7z8wu95e8u9W89Oao2MdkSAMwIevPv3081qPXKPxFMiqnbh2zQz+ZngFlpYbNQe06JoSx4q6sSyPUUUksIukWZokFRyPljrXcGqgoulYbrhjYn6MjASSM0zLLisyQZnwSBoDce7q2ZngFH.ne+9IIYmc14mU26mhDj7RsvcaRd8Wzuh4a0WRNUFrO9FrxzlxTl9UcUW0rLi0iP+F0nF0Q5t6t+Cl45ryN6D20ccWn81OQVByJyq5QM4kWd2htigd7xu7KGpyPeNsqU8ZRqmjLo0STQVYk0Yp6Xvg51.vL0cPzaomd53q7U9J87m6KkTR4s0Y7Do74yW+lxUiVEmEVVBGjX9JlqTpiBf02Vask6pV0pLsQb8nF0n9ajbzl05SnWMzPC+2l45KkTRAYmc13QdjGAjD.Xwzg+nKO5QOZJ.3mq63..nrxJ6WdK2xszRHrnwAiI2FmDy5KrSzjVOQKKV2AfSCIOM.XpyLrlgDRHArwMtQ3xkK.iIbMmz.97DJszRsrYs4JpnBIGlK5Ww7ULG.7HG4HOaZok1wlvDlvtMiUne+9+hJkJekRYk+5ZQTTKszhoer7e+e+eGqYMqo2437q0rKCqzjm7jWqtigd7DOwSDpUNwx9hVSTWlz5wtWwb6XFiw1hjI.f2S2ww.I3DragJkZu5NVhTc1Yml07bxPxiGOx7fhneMRnh43ge3G9RMy02G+wercdVVSDAF6XGqozEm5sDRHALyYNSDe7mXVHecjzQdMW4kWdx.3Wp63..3fG7f+h6+9u+VCkk8odpmxtW4z9iY0h418mTvB0c.3v7P.Xb5NHFBeCcG.CGiabiadVUY0RKsHULWzuhenWDmMRdAczQG2gYtNW3BWnoMkYKrGxHiLrhJIjR80Wed.nRKnrLU4me9eKcGC83Iexm7WDpKa5omdxQyXIJwr5i4186um.IcISK4CMRtHLHynv1DOmRopP2AwvQt4lqkkJaas0Vcjc2GQzmir06BUjbl.3cRMUSM8h6CNz9Omn+EbvAaISXNidzidoVQ4XlHYRvn05ztCdvC9yC0VKG.HqrxJ8nY7DkXVsXtcuh4.1+V0W6HYx.3+U2wQH36p6.X3n7xKO43hKtKvpJuUrhUH0iPzuhYqXNIiG.6HJrpcbs1oXv4wimoB.qZlHzT6VUVjqC1j6U7Nuy6bugyxmXhIZ5cQIKvHkVLG.vI9DMrLAazfmB.on6XYH7jJk535NHBUj7TF6I4me9KyJigBJn.qr3DNH1hurMJ4b.PzXfbX2mBjEgGUxIm7u0BKuKyBKqgMRlH.dLcGG.FsV9MbC2PH2Z4..YjQFNwLmjY0h4Ng6uKyVxCtKD.qV2AQH3Gn6.HTErR4KqOulB.+J8DQBwIyIbi6HUlQo06SDkVuBMvue++e.vEYgE4n5Iu56PbMvlbehvs0xA.RIkTr6CVt9iYMFVbBmmYYYACmFRlB.dMcGGgf+KkREJotT6hoF7e81XAfokNkCEqYMqwJKNgChs3KbiF73wSog5xRRzc2C8.jtrxJaBJkZmCq.SXWn5pqt9Ztb45Opgx1QbcWvzy1Fzcb..bfCbfvt0xA.xHiLlXzHdhxLqVLWpXty1uG17IIJXbt5Cp6fHLc8.XR8409xVcPTXgEZ6u9jFyxrYSxYSxUQx0Qx+FIcSRujrVZLyJWPv2aUd73YVUUUUxSBaXvIzGDiHacqa06EcQgVCgVRIkfbyMWjYlCdirOiYLCGSenSLvH4bBDHvK6xkqooix+i9nOJN.3DxDEqF1jAm269tuaX2Z4..ojRJ4axgRTmGOdFI0h4Yn6.vNhjmObFcgkqQoTlUd2OpqvBKLY.bG.3I640B9DLuKsET1.A2GjB.xE.mAL5pOWI.FpuibLA+2bQvyWSJojPd4kGHYc.39AvyCfxUJkjdHCQNhVtKRrjkrjqMTW1st0shLxXn+9g8t28NpgSLIzORd1.XO5pR4..KbgKz1OZ7CN3o2ntiCfHu0xA.RIkTNMyNdh1H4HozwpSLq4DUEbbc3DFKS0.iJc4Xje94+eE7+d187ZM0TSYAfIX0wRs0VqVp+EIiqgFZHKRdlj75I4VfwjZV6.3P.3uBfuOF5JkOTFCLl6KND.ZgjeaRJWuGBhIqXNI+JokVZgzLCXWc0Ept5pgRMzMtzrm8rqijNwASl3S7mzc.TXgEp6PHTbYvljIHhzVKG.HojRxR62nlARZVOMEmPKlKeQ8o59fy3IIrJmTqf9QezGsr7yO+6L3et.D75ijRJo4pi3YricrVQ8uT0UWcYRxyhjeKR9F.HPN4jSS.3igQiurJD8exnoAfeC.Zkj+bRJcgsAQLWEyCl6x+yg5xWRIkfYMqYExq+N6rSouS4PsgMrgD.vr0cbrhUrBcGBCpfYsfMo63..Xe6aeQbqkGj1dxHQpt6taoEyGghjmF.tEcGGgf2C.Eo6fHTQxIO+4O+dmK3iaSaZSYA.jRJo7uoovxz6JwkWd4I6wimYRxqljalj9G8nGcy.XmvHwUXkI5fAxsCf1BFiwb0A0LDSsSIX+j5kBmOyG7Aev+S94ment3s+xu7KervMtD1CW1kcYeFcGCAY2aIyOK.rE4+6MsoMEwsVNbn2eyDawbm.okyBJXkTbBcgE.fUqTJaeWxqWpF.ePuegEtvE9MqpppFC.zxrZb6s29vpUpKrvBiu1ZqMOR94I4iPx5yO+76LojRpD.7GAvUA688.+i.XajLZkA8D1AAG4vgrt5pq8BfrN5QO5Psntat4lOOoar3b85u9qmFI8DNmeDs7W+q+Ua6ioljwQiQbu1s28t2Pp6nMHRT2aCQhlZpobLoikmmt2VBA2tYrsFKfF82Wm.mVVXA.mHCi7Z5dmWOdoW5kB4LFEIi6nG8nixqWuKlj2NI2ulCeyjWRp8mjscRrVVY4ABmEd6ae6eU.3a7ie7C0hNurxJKIir3PUTQEkvBVvB9.XSR8X4kWdYCfgS2yHZZI.P649aRNbasbrwMtQaww6vU2gRtaMD3ymOjPB1hjpyfwVLNFzMRlErIC15gfO.7eMjKkMjRoBPxqB1j68lRJoLZ.TU+7Vp5pqtLF8nG8zAvRAvWA.Wvjm7jsz3yBk..1KIWgRo9G5NXrCryOlivBMlLFBmo678r3Eu3c+C+g+PWIlXhC1x8ycRS0vhS19129xXAKXAeL.lmtikdblm4YNecGC8G9IS+2Z292+9+o2+8e+Cqu.cricrNxwCRf.ALqJla26xT..Iq6.vl3+Q2APH5pTJkGcGDQBRp9W+q+UN..G+3GGEWbwZMdl6bm674ImgTtAR9pjr6f8K7c.fGE.WfVCTqSgj7Sq6fvNHloh4.XQgyBWc0U+M..ufK3BFzVUq4la1Q9X6Foq3hKNQe97cEm9oe50Cav.9r2RJojteZOm8OOaXSFrjaZSa59FtqijRJIaaWFZv30qWSoOl6wiG634X80H9JlSx4CfqP2wQH3iUJUXMFtrCHYJj7sAP2KZQK5n..MzPC3gdnGBj5qaxOgILgMgSNCo7j.3h0V.YOrURlutCBcKVph4eyvXYOdd4k21A.l+7m+f9nTSJojxjjNxVdajnfs7P0yctysq3iO9+BrISPN8wLAfsJ+ZG7GJXKxDK6YO6YX2Z4..omd51hAvZ3JPf.lRsE762uTwbaNZjAjdUcGGgnKC.NoA7YO9Y.3jREVYjQFXSaZSX+6e+ZJjDChcPRGY2PzrDSTw7BJn.W.3ZB0kuzRK8lPvavL1wN1AcDAmbxI+FJkpygWDJrPEBMLYQDAra8s1YAfyR2AAIwi+3O9uzLVWomd5iwLVOVMOd7XJckEOd73Dt+tc65.q1MBavX5HD7yUJUk5NHBWzHS27856qmat4hm7IexPZhETX4xF.+NcGD5TLwf+bQKZQgThHuwFaDG3.G.O8S+zmHkTkXhINTylmeA3LakfQb7506E..Gwry5G8QezQzcLzGaP2A..PwEW7O8we7GuMyXckRJo3D9AZmBud8ZJ2uoqt5xkYrdhxbBwXTQvA74uQ2wQHnc.7+S2AQjn4laN6rxJqS40iKt3vMbC2fFhHQH5ZH4uPoT6Q2AhNDSTw7ryN6PZBB3Mdi2.e0u5W8iAPu+h+ALkrzc2c+xtb45fC23SD0o762+Zb4xUHOwRoY6agKbgMq6fnGjbx.3BsAwAdhm3ILkVKG.HszRaRl05xJUas0ZJsXdGczgSnRuNgV0OZwIjEV..tLkR4U2AQjH0TS0VLlYDQjMCf4hQfMLZrvMEiK6ry9GDJK3jlzj.Ll8q5soNPKeM0TyH5GmhSPIkTxLH464fpTNf8al8yVL.mMyVKG.H0TSMeyZcYkps1ZMkuHpiN5XPS2T1DNge7foKXda9pzcbDB9..7+NjKkMUBIjvJ0cLHhXyljZu6UpCN9Jl2byMOCDhCvu4Mu4gK9hu380mWdlCzxmWd48WH4xFNwmv7QR0QO5QOMR91ybly7P.Xw5NlBCsBf2V2AQOH4X.vpsAwgo1Z4..ojRJ1pAXanZ0qd0l0f+zILvJiIdpsgif86YmR1M4K6vlgOOgfi8rertiCwvxSA6+LksoywWw7LyLyqOLVV7m+y+4b68q0VasclCwG62FQAlvTPREImVu96wBf+9jm7jOH5yHs2gX0JkxNMkqem5N..L+VKG.HwDSzVklLsZABDHccGCh90p.vLzcPDBdHkRUstChHj5Juxq7W.YvE6zMut5pq4p6fvp4nqXdvTM0OLTV1ibjifibjizZZokl6d+5s0Vam8P7Qkgssdc1d73g..j7R.vwgyrB4..6C.uwPtTVDRlNrAcqlnQqkC.UbwEmSHaWbJLqVnzmOeNxrRSrrfoAtBzcbDB5thJpvV7i1CWG9vGNWR9lwGe7gTcCD1aIlXhOGFg0p4N5JlCfOEFhsgN6rS7.OvCrh7yO+3yO+7yLgDRnvd81w0c2c2u+h5t6t6eoGOdlAbtUBLlP2c28C9ge3GFn1ZqMC.7J5NdFltDa1iE9aq6...XW6ZWldqkCm881LqtxR9lw5IJyutC.K1+ErmysBmj.ABb8SYJSwwjlfKpnhR0mOeeFRtsoN0odL.7Y0cLILMeJe97cQ5NHrRN8922sNXuoe+9wO4m7S97268du+iAXQRxkq9crGcr3hKtaOkTRwNUIpQbJnfBRLt3haEKaYK6n5NVLA+TkRYaRQhEWbwIB.ytUpCajD+1e6u0zii0st0Y6q7SzlOe9VttiAwmH334vIzmmaeqacq+AcGDCFRFWGczQtolZpKG.+G.3bzbHIhhhO93+qEWbwYeFmwY3HyNPgKGaqJQxDgwjyv.ZG6XG0bu2689lCxhjd7wep+1jibjirdaVKaNRjZAKXA+TcGDljZfMKO.O24N2uDrAW+GkZsbje946DF3i8Geu9q+5ogge1JIAud85DxnAijtOqsXl0cnbricr+sUrhUXqdRFjLtVas0wSxKijaF.ARM0TqD.+AHUJejfTl6bm6Cn6fvp3jaw7yenVfryN66.C9M9GcRIcpy7qG7fGrpgQbIF9b8tu669ql9zm92Q2AhIY4Jkx17EcAGaF1hbn769tuaToU6SJojRMZrds.IrxUtx1HI.P2vXbI7Q.XO.nX.TN.pB.sNXMdPiM13Wu+lXUrgrMWWDMEL8HdY5NNBAsNwINwWT2AAIiqs1ZaLomd5KD.eM.7URO8z0dCIHzp01UWc8emTRIsScGHQaN4SzuqA6M2111Fdxm7Iec.D292+9+4s2d6Wd+rXSJ0TO0u+9y849bCXJTTDc8XO1iM45pqtCrzktzXkJk+iUJ0AzcPzGW..RS2AA.v2869cakggPc8NpQMpLilwsEINXLAa70gQ2N5uAf8BflAPWj72RxyIX52C.mHKFciYmc1aTobDiWptzc.XAT.3un6fHTzVascS.PKYMp5pqtLI4xI4FAfuzSOc2v3b9+OvYWWEgIIwDS78c61sjsoriHYJC0Wfum8rGBf3eoW5ktmfuzozJ3qbkq7lFfOdsjzQ7sZwPR7fG7f+jvoRZN.6k8pRS1DJRdHcuiIREhaiwcvCdv6V2wpEp38rm8bdd858BHYw5NXBSw7oiVRdQ5dmbnp7xK2R6BXszRKikj2HIOpt21ENFai1uuW0T4T6JKCUJNDG4HGAKe4K+KuxUtxdR4S4AiVt3De49YbFmwBFfO9X.vn.P8C2.UL3JnfBbcVm0YspYMqY8L.3T6WQNaKSoTlxzqtYgjmNbF4P4HxS+zO8jV0pV0qkYlYNRJ22N24Lm4rMcGDQnX5VLmjwCfmW2wQH5+dZSaZdrhBhFyMEOC.NOqn7DwTNO.bc.HlclY2oVw7u9Ps.ae6aGemuy24OmXhexrR8JW4Jmwa7Fuwg54um1zl1fMqdNG.r0gUTJFLwsqcsqO6bm6b+SwEWbwh4a4KQoT0o6fnerdcG.QIwWVYk8il1zl18n6.QDVh0yxBWGbHyEFG+3G+WYEkCImOLF2DBQjZij7YTJUG5NPhFbbONffOBiAc19r4laF24cdmXIKYImzqe+2+8ePR1SqjG2TlxTFr9RdNCuHUze1vF1PBG7fGbUjr14Mu48FwnUJ+ITJ0qo6fnuZs0VGG.h4xGrEVXgmgOe9pRpTtijOcG.QKjLU3fl4nKszRKwhJpGwhJGQrsqQ2APzhSrEyGEFhIng2+8eer10tVLwINwS50Oyy7LA.tYXzJFIMgILgAbczVasU9vNREmvl1zlx9hu3K9aNgILgeAbf+fvvvA.vZ0cPzeRO8ziolI7JrvBSd9ye9+pLyLyAMsoJr0bLShMQf6V2AP3XIKYIVR2XA.KzhJGQrs6F.OotChnAmXEyGzbyqOe9vi7HOBtu669Nk2ijn95q+BC9midbiafmwtSO8zeGRNwX0GUhEItst0sNuy4bNm6NszR6JzcvXA7Afy2NkZD6AISF.+.cGGlDU4kW9Ele94+J.nem4dENFwjULmjiF.NoeHbSv5xo7aG.K0hJKQrK25N.hVbhUL+hGr27.G3.X4Ke48z53mj29sea32u+dpMd94latC1pJaXLfQOzfsPhSghj4AiGyzOAwdCnyAyBTJUS5NHF.eIcG.lgssssk4BW3BeVWtbMn2GP3XDSVwb.7D5N.BSV4fG9mBfW0BKOQro0o6.HZwI1kBt1A6Mm7jmL99e+uO5aN70iGO31tsa61xLyLON.vO5G8iVX+M4B0GZIet5zPRWjbFj7NHYq.nRXj2kGIUo7qUoT6V2AQ+I33x3wzcbXFNuy67ZVpTdLkXtJlSx7AvUo63HLYk8E92.FSTVBQjpADC+i6bTsXNISDFoxvAz.Ma20XiMhO5i9nGnqt55o..V7hWbnz0JZIrCxQHBlFvlG.9V.3Fgy7G4YVd..7T5NHFDyCFOAHgvtwp5WyVBZL+W7mzcbDldE.XYy1mJkpaRtD.TsUUlhXNWncrKiZVbZUlZTQ5GriN5..fqXEqnF.3ZQKZQW3P7QvAO3AaKRKuXUjbLj7dgQ+od6vnh4NsyiLSOG.tsAa5Q2F390c.HDCfXpJlCfyD.KV2AQ33C9fO3pTJkk9zgUJUMPFDnhHyMpTph0cPDM4zpP0DG5Eo+0c2cCDbvsbm24cNqd2+xOzgNDd5m9o66Gw2Lm4Li0ywtgLRFmOe9VE.pE.2ltiGahWG.eU61jHTuQxrQLXJRTDyHVZv0qfyYxD5DrvrwxIQoTeD.VxPtfBwm3tPL7DKTObZULOhmwByHiL.LtwI95e8u9C2yq62ueb0W8UC+9OkmJxqayaETqfpkVZYLj7pAPUwGe7aQ2AjMxqCfufU2RSQfUq6.PHFDwL8wbRtL3.mUcau81OWcU1Jk5C.v70U4KbT9d.3mNRndYNp9XN.lUj9AC12yUeuu22KuYNyY94640c4xEl9zmNF8nGce+H1tIHFyFIOW.7+EFsZ0G1VasUSiM1Xxomd5yHmbxY4.3KhgHmwOB0+Kb.UJOX+c8TyanBg8QLQEyIoKXzs1bbRM0T2FIALpzyO1pKekRsCRNG.rWqtrENFKWoT+u5NHrJNsJle5Q5GLkTRA21scayadyadmTBouqt5BiabiCSZRS5jV95qu9X5oLXR9M.v+SueszSOcjd5oqm.x4nb.rR6dkxCZ5PFzmB6stzc.XRtbLDIl.Gf+ktJXkRsuFZngyLmbxYW5JFD1ReL.9bJkpNcGHVImVWYYX8H2t7K+x2YhIl3h58qcW20cge8u9WioN0odRKaEUTQECmxxNhjpctycNt.AB7znOUJWDZZt4lWnRobJi8.Yr.Hr6b7C9yfYnJmVlX4TTSM0720Y4OpQMpcWas09EzYLHrM5F.ec.L+QZUJGbnOqfB...B.IQTPTwA0h4Aer7yb3rNV5RWJ74y2I8Zqd0qFkTRIXTi5jS3Ku268dMObJK6fdRogABDXItb45bAvpNqy5rjVPMB8pu5qtpK8RuzFzcbDJpt5pSCFowRgvNywWwb.70fyeFn84xM2baW2AAIKQ2wfP6dfJpnh6ZJSYJwDcysHgioh4.HUyXkjPBmbWld9ye93ge3G9TlPhBDHvIWCdGDRlXkUV44.fBAPJtb4R2gTrficIWxk72zcPDpF8nG8OP2wfPDBbzULmjIAqcx4Ipn0Va8tzY4WTQEk5BVvB9ovX.9IFY50polZ9F4latGW2Aht4XpXd0UW8T5cJNzr3xkKLsoMsS40862usta9D7KDlUWc00LiKt3lbBIjvTfQeJd4.H691m4ECaeA6bZQr21wN1wDSHgDtacGGBQHvo2GyuI37Gf70kYlYtecU3jLKXLSNZq+NWQTSms1ZqWRlYl46ffoz5Q5bLULulZp4RiFULefnTprgMJiADrq7nd0W8US3bO2ycF.Xm.HgjRZjzrdu17kUJ0N0cPDJJt3hSetyctEo63PHBQN4mLYp.3gGxEz9asPuUHpE.bq.3WowXPnGu3gO7g+JSaZSyQ+jyLapgdQzOR5xsa2MN9wO9Lrpx7C+vObMm24cda1pJO.isyZqs1wkPBIL6LxHi46xkqECfE.foNTeVQTQW.XwJkZG5NPBAJ2tceliabi6swvXFxUHrXonTJG4WJSx0Af6V2wwv0wN1wRahSbhZehdhjSFFyODyV2whH5ymOeqMwDS7wfzJ4mBaeEyC1Rw+FXL0uakJWoTy.VvIMAugzOA.WaztrDgr2qt5p6RF6XGaq5NPFDtdu268F8Ye1m8hSM0T+Y.Xt5NfDhvThJkxw0p4jLc.Xmu2Pn5YTJ0Uq6fnWhij2A.tGcGHhnmN5niqLszR6up63vtxITw7YC8MwCLakRYp88NRFG.heO6YOHszRKtbyM2omTRIsGyrLDCaWqRodJDF+nrf+.xD.PxA+WR.HQ.jPokVZ76ae6yUSM0jqFZnAUqs1pJkTRIN.fTSMUjXhIpRIkTPZoklJqrxRkTRI4JszRykKWthO93iOIWtbkbBIjPlomd5iJ8zSehIlXhmN.NeHsLtv4KdGxbBvIgj2G.9g5NNFt7506YlTRIsacGG8UGczwpSIkTJP2wgvz4C.WXvY7Uw.vITw7aG.+bMU7aUoTKa3tRZokVFSFYjw0.i9xmiaJadDjc.fOuRopMTV3fUF+7fQ9BeUPF7RB8xGLxpEEb3Ce3Ve1m8YW0sbK2xylbxIq63Z.oTp3fC6QYSxrAPi5NNLCadyaN90rl0XK+gQd85csIjPBR+Nef8B+q+0+5NJszR6X9ye9epYMqY83XXlRoixdN.bcJkpMcGH1cNgJluMXT4GsnyN6bpolZpGML9Htd4W9kybZSaZ4OyYNyKLgDR3lgTYbmfqE.Ocnl4UJt3hSbtyct+E.boQ0nRHFZcCfa3i9nO5OrvEtvSpagzZqstszSOcsc+yghpu4oVG.R9X.36n63vD7+nTpqS2Aw.ofBJv0pW8p8q63vDU2AO3A+Q6bm67cG6XGq6Zqs1N.Pf0rl0zMN4ebpB.pMrgM3ZAKXAIjSN4jQN4jyDxImbNa.bM.3hBtb4oTpp60mKNRdq.3AsjslPS2.3Q.vCqTpJ0cv3TX6uoHIaD5cZE+upTpqL3+20C9fOXhWxkbIolWd4kUVYk0X.vDAvoCfyA.W..lfYV30Vasn1ZqEyYNywLWshOw6UQEUrpoLkoDRSbPjLtJpnhEL4IO4WCRWIQnYABD3GWQEU7.CTVMnwFa7Kkc1Y+7VcbEhNtRoFutChvAIGE.pW2wgYnwFabUiZTi5kzcbLXH46.iuW0wqrxJK6YLiYLrm3BKnfBbcwW7EOpLxHil6uYgZRlC.tcnutZU6s2d62yt28te1Eu3EeTXT4bQXvVWw7fcU.6vA0xAvolrys.ABD.20ccW3RtjKAe5O8mVGgPrpt6niN9xokVZaAgviRO3XC3GCf6BRWVQnYd858I1wN1wsc9m+42xfsbtc6dFiabi6PVUbEl9XkRc15NHBGj72hXjYT28su8sf4Lm4rccGGCFRlL.99.3N.PZZNbFNZWoToakEXgEVX7yYNy4LF23F2U.i7s+3hxE4583wyuN4jStbmxb9gckcuh4wA.aY+eyJ4wiGroMsIbC2vMbJybohHxGbvCdvKdVyZVCZkZ5MR9qfwXDPHzom4i9nO5eegKbg0EJKbCMzPV4jSNMEsCpHzqoTpKQ2AQnhjiA.gz3OwInzRK8KeZm1o8B5NNBEjTc3Ce3jTJUNokVZSZLiYLmOLterctOUeBc2c2OmKWtVsNiARl..xDF8.grgQxIf.noCe3C2T6s2da6cu606d1ydh6ltoaZlie7ieWgvp8ifwOb5CbhYWI6JacEye228cyXoKcogbkmh042ueDe7Nl4DJ6peI.tiv4WzSxyBFSnSBgt7h0Vase6wMtwU8PunehfiEB65rq4SpTpuotChPEI+8.3qq63vrzc2cWiKWtlHrGOU5HRmc1Y9ImbxuG.xS2wxfokVZ4GjUVYYm562CpxKu7jyO+7GnIXQev3oGuIkRcbKLrFwvV+H4at4lWjtiA6DoR4COABDXsJk5GElUJWAf+RTLrDhAyKCfIqTpuX3Vob.f4N24ZmG7bGS2APnhjiCwPUJG.Ht3haBjzVWg1gRJojxgO1wN1Yo63XnzXiMd.cGCgpBKrv3yO+7ew94sdV.LeXLof8KkJkG8XaqoGIys5pq9k0cbHhY76hO93ezH3ycFPSiu.wHZuH.tYkRUwvYknTptIY2vd1HL0n6.HL3XZsyvv6s4Mu4v9G6Y2LwINw55niN9cojRJWutikARqs15v55XqRvAN56gOY1W8OBfGC.aWoT10m7VLG63Mq6oUJ+a4laton6XQDSn0ZpolaIB+r+XSMRDhA2yBizf1Wb3Vo7doJSZ8X1Bo9IutEr0x+Z5NNLatc69KaWyg4gq8u+8+eq6XXvzZqsZ6OWO3LP9c.faqgFZXJadyaNdkR80TJ0GHUJ2ZYWaw7ICizOnPLbUJ.VXt4la6g6GL3fkQqCXGwHF+u.3pUJUzn6cbLXbOU6lPJEkZCDK1Z4u4DlvDbq6fvrzUWcUptigAS6s2tseR0IXCA33mMaiEXKawb.by5N.DwDdH.b5JkJRyJE4ahwhPzu76++e6cuGeTVcs2.+2JISlbkbgPBPf.ALnHAQJ3EzhZphWnspUKwVsd9T6QqZa8zSa0Z0dQssGeaea01i5odrpG8T0d7jz5EN7pBpPDEE0DEABfbK2SfDBSBISlIyjYVu+wL3IDRHSl444YuelY88yG+HjLyduRlGRVy9YsW6gVIQTElTR4..5Z4Jn8mdlwqqV9.CLvco5XvHwLq0MIBWtbMVajRg33ncqXdUUUUx.3Go53PXq0I.t.hncFiiyYZDAiPLVZqs1V7LlwLL6N9iVdf3zbyMGyG1JVfGT0AfYHiLx3STcLXjV1xVlV2p9prxJ04MgsPyncqX9ke4W9bgFFWBai+YDpFci0jxA.NeCXLDhwzN1wNFDleaqUKqu0csqco02del4hPni.83MOIQT7Vhh5bKebPn2wmPyncI.6zoyqV0wfvV5k.P9DQ+qDQF0FZ50Av2bqacqyb+6e+yxfFSg3yrhUrhcvLGjY1Ey7qwLufva9cijVVxHd73Q2u89+AUG.lA+98+npNFLAi6o2rBsCUG.B6EcqTVH.b6pNHD1J6D.WsAsB4GChnpG1eMYl04e1uvF6kZrwF+AkVZoMAyIACsr9aSM0T8o5XXrDd0x+5pNNLCM1XiaW0wfQiHh03e97tUc.HrWzpDy22912j.P9pNND1BGF.WEQzFgErZI+g+veHaydNDIbFzmOemgSmN2lIOOZYh4qbkqTmqK33xUKG.qYdyadwqs9Ncse8q0cLFg9QqtHdNyYNye7eThDbtAvWA.ERD8VvhtElKdwK9aYEyiHgR4VPR4..8YAyQzPKq6Vl4og3zUKG.2upC.Sjt9FNZU0AfvdQqVwb.rRUG.BskG.7s.veyp23RO+y+7kedm24EO1KiEpyyRDsWKZtzwDy04SBwn4DB1tnVUG.lHccCspqsqTglR2RL+qo5.PncbCf+A.rZqNg7G6wdLGW5kdo2XIkTxexJmWQ7uibji7KsvoaBe3ZYA1kpCfQCy7L.vUo53vj7vDQ5b4CEqz0u1zx1UpPeoMIlyLmL.JS0wgPazCBkP9qXfcYkHFy7B.Pc.voUO2h3eqcsqc+V3zoic+DcstaeLUG.ln+npC.SlVVZTPS6JRB8kNUi4Yo5.PnEZ..U.fBHh9eTQR4gkE.1.zzd.svV6uUYkUZkWWqiIl2npCfQZfAFXVH9sbJ6Dg9Yqwyz0Dy0xMesPeoMIl6wimbUcLHTpM.fERDMWhnZTXB4..fH58IhtLhnoPgj7u427axeKaYKUDHPf0nxXSXuM3fC9ea0SoEOeQh1Tc.LRomd5OkpiASzOfHRa6mfFDcMw7ATc.HrWzlDyegW3ElgpiAgRb+82e+EQD8EHh1Nz2CJhf20ccWtV7hWbMojRJe4CbfCjE.tAnmI8HzXG3.GnNKdJ0tqQGZng5T0wvv40q2xPn6TW7pUq5.vBnqIl6U0AfvdQaRLuhJp3yq5XPDcd9m+4uZ.jL.RdvAG7JhfmR8.3Rpqt5RkH5mlc1YqU+R5Hwzl1zbSD8zHTIubcPe23QBMSCMzfU2kFztCxGe97cXUGCCC4zoymS0AgIpZhn9UcPXAzt2.ZXxuaPLgXzG8yQMl4FAfbrmqHtc6FYlYlviGOXKaYKHqrxByadyCNcN968Qhnjw+6pUPLyCgi+M80N.t21au8Wr3hKNtqtsYlSC.+N.78TcrHzZCBfzsxxJfYNS.nUIl0UWcMuBKrv8n53..nu95agYkUVaU0wgI5TMiSFYcCy7t.vIq53XTjhpKMSg8hVzUV5omdxGRR4J2C8PODFbvAwO9G+iA.v0ccWGdzG8QQ1YeBOzKaBG6sPjAv2D.eU.7Q.XiM2byaojRJom34ZbjHxK.tsAGbvGK0TS8cAfbRgJFMqVA+6.sq+N61saco2pSYkUVVcM+akNDQjV1ZJMA5ZIinqkXiPSoEkxRN4jyMp5XHQWlYlIV0pVExKu7vUe0WMJpnhvy8bOG1wN1w38TOt1KFQzyPDcEDQ2GQzFl0rlkq34jxGNmNct8VZokhPhQMcJl3dcELmZ2p0ELXPsnSwzau8dl.Hd9Dm96C8ce6XzzwtehmDke2mv3n7RYY6ae6otfEr.cs1vDiuBHhjCPgQAy7jAvkfP2AgKDG6aDNX2c2cI986umoN0otW.LUEDhBKle+9OiTSMUK8zWjYNInYImWSM0jdEUTgRWgyveeoQ.LSUFGlrLIhRH5JHLyuH.txd6s2+obxImGR0wSX0SDUtpCBg8hxWw7ErfErTUGChn1uPRJerQD0MQzekH5hIhRo5pq1YM0TS1g6nKNJnfBZaZSaZtesW609hpNV0TuTe8027qu95cBfjB21Jo8u+8aaaspc2c2MqfoUGWwNkWdMc2c2WDhuSJ++HQIo7vZA.ORN4jyin5.YXZQ0Afv9Q4qXNy7CBfefpiCwD1l.v4QDI0OWrKElYYm6++x+QNxQt5bxIm+mQ6SVas0lwRVxRzwiY9wUc0UWpKcoK0xeslYN.zfEh4nHhRBp8MLjLy7g.fs8M4EAJkHpQUGDVEl4qrwFa70JszR8xL2Lzi2z0SRDIkpqXBQG17mWtpC.wD1fszRKqnjRJQRJ2.DNooDMAAviejibj+Re802m1SO8zWlYlYR4jSNNyKu75OmbxYL+dRZoklxWs0nTepHo7vZC5QhJ.gZebJcU7OvANv0f36jx2GQTSpNHrRDQuzQ+yACFryjRJIc35cqt0nJhCnCqfxbUc.HlXpu95u4RJoDsXyaEO3gdnGJUUGCVnOr0VacY0TSMNIhtkbxIm2cFyXFcWd4k6qzRK0a94meuiWqEaG6XG102HiJOwXOfBm6QZ+pbxqpppRsnhJ5oUYLXA9NPOKgIqhtzdPi6ZMvBymRWw7MrgMnCqXuXBpnhJpdUGCwSV9xW9on5XvrELXvWYm6bm2Z4kWdLWi0UVYkAY1VlywFT3bePEN2iz9T4jWQEU7OC.GpLFLYAqqt5T40ZJWRIkjtrvQRh4hILklX7hW7hkdWtMTAETvivLeYDQ8fD6UkwPTVYkcKpNFLQ02RKsbkkTRI60.GS650behBmacJwbksg3dhm3IxtfBJ32pp42h7yUXISoKzkdZtNcB2JrITVorvLS4jSNGWOvVXKbV.3vLyAYlalY9JTc.YW8XO1i4HqrxJtLwbe97c8.3zL3jxOJcpzLhTprle6Tgy8Hor5tcUqZU+AUM2Vkd6s2+cUGCZ.c4Mlni8VcglSk0X9BPnd6rvdal.3k74y2RXlkRSZB5htnK5rTcLLN1Vas0147xu7KOom7IexIsu8suudD7b5bfAFXFNc57YMwt1Salz3ZlbopINPf.ZSaM0qWuJ4MUs4Mu4YLoIMo+QUL2Vn0lat4JqRKfOUG.goK05tvFQkIR8iT3bKLXNb3nV.fv09aO.Xa.3sAPsd85ca0We8sujkrD4TPaDlyblyuW0wvXIPf.2PJojx+IN1RG44c4xkubyM2+9X7zVS80W+UWd4ka1+hw1.vRL44vH4lHRYIK31s6COoIMIUM8GC2tc2kBlV5LNiynJELuVsaW0AflPWVwbaYacUnVJYEyYlSFgNMDEwmxE.KG.2M.dgzRKs8DtuSeXl4GfY97b4xUtPC5i9pzt10tJ.gJKHsie+9WZJojxSiQodte8W+0eY.LxSqW+.3ZHhtbKHob.8ploiDprirf96u+dT47Obd73wxuyAG7fG7bSJojVlUOuVrNIhjMleH5RmaRW1DpBaDUUJKknn4UnV4BfeH.dqbyMWWLy8xL+uvLuHlYmpN3rZm7IexeWUGCiFe97UdpolZci0muxJqL..90.nj8u+8OK.bpHzQ+cUv51Xl1sDyeSUN4tb4pWUN+CmWudszXY6ae6oVXgE9JV4bpH2LruaLZiltbVGnKaBUgMhpJkkKRQyqPujMBsp52M..y76CfeC.1H.bEOW1K6d261I.tWUGGije+9OKmNcFIq51ukHRk2tXsoloiPaQkSNybepb9GNe97Yowx7m+7e.D5m0DWq95qOQ3MeDozke2gtTq6BaDUsh4Z4JEJTtyB.uHBkzU6Ly2Jy7zQbXIuTVYkckpNFFogFZnKI0TS8ChjGqhSJGv9kXtpOEF0lMglOe9Fvplq95quElTRI88rp4Sg9UVTIjIlXzkUtWXiX4IlWas05..KxpmWgsyTAveB.sEtjW9gLySmY11mjNybR.P2ZoYeSGNbrtH4AxLqCmVuZSMSGgTVGYA.HojRxxRFd73ymOK416WUUUkbVYk0aXEykp8oe5m9PpNFzLZwumn95qWWp0cgMhkmX9RVxRJ1pmSgsW1.3APnNwwAXluQl4BTbLEKNaDpd60E+bhn+yH4Atu8suh.vEaxwSjPapY5HvgT8cXHPf.ZSstNv.CLxMNroXUqZU2J.JzJlKEasmxobJxIL4vzSO8jtpiA.fcricHIlKlvTQornkcgBgsQg.3wAPWLy6kYtRlYaS8iFdE+eZUGGCyyBf+kH8AOm4LmBIhzgU62Ncvc7hpN.BDHfkjLbDxzu89Ly4.fG1rmGcPe802OT0wfto0VaU4Il61saTYkUZVmiCh3XpHw7JUvbJhOMW.7eCfivLudl4kYCNjilO.JS0AQXeH.9lSjMYKQz1fdrwpzlMy33IPf.qWChAcp9ish5t8Qsf4PGz9jlzj1opCBcSJojhxWrFud8BnG+rRgMiUmXNA.kto2dm24ctiMtwMdt6e+6eEd734OoxXQXnp..uK.FjY9ewsa2Eqo0i9eV0APX8.fKfHxVdqVc61ss4f6n1ZqcqpNFRKszzlMgVEUTgotJhLyk.fH4DpMdvMAI4uiSVYkUNpNFRN4jsM+LJgdwRSL2kKW4X0y4Hl+WX4Ke4+9y+7O+2ctyctuQFYjw288du26VTU7HLEIAf6NiLxnU.7oLyWLyrCUGT..LyyD.mqpiivVLQj1rg.mn74ymtcvcTeGczwrIhRlHJo.ABbMG8S79u+62pJCL.sqVWMsDIC+lw+ql03qYBBfWW0AgNJqrxR46AorxJqCn5XPXOYoqnHy7R.PsV4bNbc1Ym2cQEUz+mg+wB2gLtdDptk0hD3DFtf.3WAf+MhHUbbfC..l4mG.Wy39.MeeChnmS0AQrH7ARktrgFOP80W+rFd6paDwWxDQptVSIlYUGC..dHhxvrFbl4EB.keGJrH2AQzuW0AgNZngF5HImbxptbVdOhnyQwwfvFxpW85OuEOeGiLyLyTG4GiHJX3NRQF.3KB02ugEFuj.v8.fNYleCl4E.q+MklOzfjxaokVdahn3gUTT08Q8Oy92+9+GFkdH8Q+6ukFjTNPnUoVGtKCsahiMAfpMwwWq3xkqGW0wftRCRJGv9cVKHzDVch4JMwjLyLyw71aQDMDQzq.fRAvbPndnsH9yEBfsyL2.ybELyIaQy6OwhlmSnlat4uFhCpI0vI6pEcZjppppi6t.NrMT6e2hCmSjFUc..fNMqAlY9yAfS1rFeMy+U94mucpkgZYBeWv0AGV0AfvdxJu.NI.rLKb9FMyX7d.DQLQTCDQeW.3D.WD.9XSOxDVsYAf0C.WLyekpppJSKA81ZqsL.vcXVi+DPOm64dtcn5fv.EQmRols1au8ST8a+NVVfL9zg6FnYsJhD.dFSZr0Q2spC.cUM0TitjXtROTwD1WV1EvM2by5vApxzlHOXhHeDQuIQzmC.4iP6.d4fbH9R1.3EV0pVUutb45pfI7uIl9zm92xnGynzOXhzZDsAdWUG...21scaSYL9TCBf8Zkwx3PGRL2TVEwCe3CWNB0JRSDTOziWK0Ryd1yVWZYt1oyZAgFwxRLepScp5vw38wUi4QJhHWDQOABc.2LKDZyDpM04pHlkYt4l6emYts1au8y.FTMnWas05..OnQLVwnWC.+EUGDFr2S0A..PYkU1LGiOUi.neKLTFO5vcKwLJ+BJu7xKQZ0x+1wYuAaCUVYkkVzDGBDHfjXtHpXYIl6vgiy1plqSfVh0AHbotzLQzu..oCfEBfmLliLgtXpSaZS6CXlWKybLeWdVzhVzWBpua+r5pqt5ujlrIDMRaW0A.Bsp3exX74tSMKAJcn8sY3snSWtbsH.rHidb0T8AfMq5fPmUPAE3T0w..Pu81qN8lxE1HVYsX8Urv4Zr7rF4fQDEfHZ6DQ2HBsZ7KC.qwHmCgxrBDp9yu9X3fJJoTRIkmvHCpnvGVc0UeUUVYk5Ter1nzrBm6N862+YTc0UmIQzXUKoq1Rinw2AUc...C8PWgYNobyM2+lQNlZtaIN7MXazRW0A..PvfAkCXHQTwpRLmPnSlQU5FfI1gDHh7SDsYhnubiM1X5HzlFcSl07IrL+E.71LySZh9DGXfANaDZuInJGB.mWbZR4fHxO.9CpXt6ryNWTpolZsmnu2pYqVNfdzkHLzjU7506J.fNTljVEcpK+nqzgVkHRJojzkyYAgMikjX9N24NyxJlmwxZW6ZOKhnm1pVogRKsTug2zne9N6ryrGZngtbL12tag96bQnUOe4SfmCkd5o+eXVATjvsa2KjHJt9WNTe80+SfBZahEVXglVa+yDo7tDQf.ALrDyerG6wbjVZokHkn5ulHRKZQnZt7Tc...je94qCma.BaHKIw7hKt3wsMEZlV8pW8NT0bWTQE0uCGN9eHhN88u+8maf.ApD.6QUwiHpkD.1Hy7uDQv+toqt5ZdPg8T4gFZnJRDNRnKu7x8cvCdvepUOutc6tPqdNM.JuuWGHP.CKFtga3Ftc.joQMd5tibji7GUcLXSLVcIIql7lnDQEKIw7ryN6SyJlmwRYkUlVz9jl6bmauojRJUSDMu5qu9I6wimqGRauxt4myLu9cu6ceB2fQETPAOpUEPih6ygCG0nv42Rsl0rFKuFiyLyLeAK7voxnn7ZdkY1P5TE6ZW6p.GNbb+FwXYS7R4jSNxIIYjoXUG...tb4RRL2hvLSLyIwL6fYNMl4LYlygYNel4BYlmJy7zYlKN7etPWtbkayM2b5ZzAR0mwpBnKzhlmQ0sdq25zU47OZJu7xObFYjwyRDUZM0TyT5qu9tQnGcMAw367Kqrx9Tl4QsVFas0VmIT2dp38.vuTQysR31saU7uaVF.doXXiAqBJurlBFLnQrh4zIexmbhzF9Dd8581UcLXiLKUG...6XG6PZmxQIl4jYlSObh0kvLuvvmT2eMl4amY9gYleYl4sxLO..BBf..vG.7fPso1dPnCzrChPsJ11.Pqg+yGL2by00Lm4LG..ABON2Iy77YlU9B4ZI+REl4tgZ2DbeQhnWQgyejhdu268Jb9ye9ekbxImeE.JP0Aj3D5v.XNDQGSxFLyuJ.tTEDOd5ryNKrnhJJQqMcQLyppSULahHawc8hY1AB8KtTl96u+SK6rydawxX3wimKLszR6MLpXxFXmDQK..51lIVKwL+x.3xUcbzRKsblkTRIenpiCcSUUUUxqZUqJc.jC.JB.yD.kBfSE.md3+S0sX3+D.9c.nIUrI9M8Dyqs1ZcrjkrDk8KCb4x0rxKu7NBQTOpJFhRza8Vu0TW3BW3UmWd4cePsuwFwXqcDJ47AA.1+92+IWZoktKUDH9746zc5zYB4lLlY1K.TQ+K96PDoxxVJhEd08UZq1yiGOyJiLxHpaykM2byoOyYNSWPMuVqD986+bSM0T0hS4V6.l4FgdrDd+vPB..f.PRDEDUp4KgH5iTcPnBgKOjLQnDumC.NMDpIJbA.PGNE3iT6C.eC.7AVYaJ0zSL+.G3.EUTQEorRzXaaaaS8zNsSSG5euwB5ce22c5yadyaUSdxS9df85B6DA+6DQ2J.RhYd2PMsuseBQzuUAyqVfYd6.XAJXpWMQzUnf4MpvL6BJ7mezau8let4laT2cXXleV.bcFXHo65C.4J8t7HiN7lOGlSmHJdegRHl4il.97Av4.fqL7eNdRKHzcg4SrhUP2zSLmY9rfZOoxlGQT7TWPgdi23Ml9RVxRtlbyM26EZROaUfj8506uJszR6tUvb+g.3rSj+k2LyOJ.tEEL0dHhxPAyaTgYtF.b9JLDRKZa4e9746y6vgi21nCHM2WmH54UcPXWDt9f0hZ61mOeKxoSmaU0wgAgps1ZSeAKXAElVZoMO.bV.3KF9+mHYs.nRhHCYSrOVrhDyuUDpdcTg0.fqHNNgE5C9fOXFkWd4Uld5oeOPRRWkN..lpBl2fc2c24UPAEXp+fBcGy7UBfWTQSeRZ3gIznhY9g.vsovPHp9dEybVHzAlUBSIr..rm8rmzl27lmzcOhPc0UWYqK+rPWtbsn7yOeaUh4LyIUe80mQ1YmcgEVXgkkVZocl.3xPnM6tHjf.3RIhdcyZBrhce5EaAywX4ZiiSJG.fOyy7LaA.O..dv8rm8LiRKszqI4jS9W.KHI8.ABfZpoFzbyMioLkofhKtXL6YOajWdZw46fUSEIkC.bg5xuHRwjSY2HyNU3b+BQYR4DBsHKITIkCfetjT9DiGOdlvmPylEhHsqM7cTUUUUIWRIkj4LlwLlZd4k2ImQFYb1HTB3KdAKPEUDnsRR.XcLy+Vhn6Blvlx1JVwbUsor..bPDMjhlaUhFXfAlQ5om9WG.+LXxIo6xkKricrCrm8rG7ge3Gh96uebdm24gktzkhEtvEhjRRa+4S1cOFQjJJeCsS3j21Ir9C0I+DQoZwyYTyiGOWbZok1ZUwb60q2xRO8z26D84o365pxTWc0k6RW5RU9gBkcRO8zyRxImbpU0wA.vgNzgNioLkonzXgYNIWtbkclYl4LRM0TW..97HTsRqCaN13AOL.99F8cL0TSLmYNUn1S+pjiyWw7HwQSR+Z.fkrR5ACFD8zSOfYFSdxS1rmtDUGpt5pa5KcoKUKpmRcPe80W4YkUVwTq3KJ7dDQmiEOmQsFZngSY1yd1JYUyewW7EK3ptpqZBcH4vLOK.zn4DQZsmiH5an5fvtwsa2e0LxHipUcb..zWe8s7IMoI8NV07s8su8TWvBVvT.vofPIfe0.XgV07m.aEDQFZ6a0rKkEUVSCusjTN..3LxHiV.vuG.O.y7L.foljdRIkDxOeo6NZlZu81WtjT9wJ6rytdl45g01cV9KV3bEyZs0VO3rm8rUxb+du26MgZatg2HeIZa1S..r28t26R0wfcTFYjwhTcLbTolZplV9O6d261YYkU1TQn1P3ECfuFjy8DUY0LySxHqNCytFCJwjG+wzfCN3uSUysFiIhZgH52SDkCB85ysiPsjKKSiM1380Ymc9qCFL3Gakya7hfAC9aKt3hURuRWywd734lsv4qS.7LV37EyV9xWtx1OBACFbhtPI+TD5vGIQyGVVYk0hpCBapUn5.3nb5zogz1bqpppRt+96epLyqfY9AXl6prxJyKBcmjVM.9dPRJWkRGgZQjFFyNw7SyjG+wTu81qbhachczjzefvIoOS.7iPniwVSSWc0Et1q8Ze4hJpneQxIm7miHJkMtwMVRmc142B.0alycbhd93O9i+4pNHzUqYMqYyvZJetRHhJhHxsELWFo..PIsO1rxJqHtNLYlmM.tWSKXzX8zSOeaUGC1TDzq122kDMOocu6c6zqWuyiY9lXl27pV0pFJyLyrC.rN.7CgjDtN5wCenJYHL6DyUVsW1UWccXUM21PLQTqDQOHQT9.XF.3G.SHI8oLkofMtwM9QG7fGro1au8atolZZVACFrihJpnmhHp7ZpolrapoltvfACpjMnltyiGOeQoDVFaUVYkA.fUzK46vBlCyx+iJlz4N24FQ6oIl4jAv5M4vQW0dd4kW79gRioXm6bmYo5XXDtzppppS3FCmYlN3AOXVLymNy7syLu2xJqLuNc57SAveF50azPL1xE.eNUGDQDl4NX0wz63LI.Hl4hYluMl4tM4WudzAFXfiozmtm64dRq4la97YlWuIO21EC.KnSJY2sicriIa1uP31s6kn5uNiVLyWqY+8mQyi7HORDk3Dy7Mqh3SG32ueU1dgs074y2RU8qeiTc0U2y9TO0SkK.Rt1Zq0QWc0U1LymDy7UxL+mYl6WsQnv.sQi5ZYS6WxygV0CU0pB2CQz7TzbGuhXlmJBsSuuGXR2NsN6ryWpnhJ5ZAfmg+wW6ZWalKZQK5BKpnh9+Bquk3oKtZhnWP0Agc.ybs.vLSdty5qu9YVd4kOg1Pi5.e97cFNb33Cr548Ue0WMmUtxUdBqwcl4bfIWNcZrAqt5pyL7c8QLAvgVHt2B.KW0whHwUu81a94latth0wwLKkEUdLUK2JPiGSD0AQziPDUH.lF.9NHzFfyvTXgEdkCMzP8tksrkuFFVWC5RtjKw8Tm5TWMQzort0sth5omd9AXDIuGGqk1au8YIIkG45u+9ucSdJJbAKXAqqpppJYSddLb80WeGPEya1Ymcj78pG2zCD802URJOpMSHIkKTrbxImulQLNlYh4prAV2lBm6DALQzAHhdTD5Dubp.3VPnik9XVxImriEsnE8ewLejVas0u48bO2SZC+yeIWxkzYd4k2ejHJqMu4MuHud89RFw7poNPc0U2oTbwE2rpCD6jlZpo20BllyeUqZU++XCbS+XERJojrztvzQMzPCcBSLmCsgOWk0DMZomS0AfMVkpN.DB.7agATIJl4uPQYsJQXvqhqXrQDwDQGjH5w.vzAPQ.3FAP6FvvmdwEW7Scu2685YvAG7eq1ZqsDbrWzGbYKaYaM8zS+q7xu7KOo1ZqsuEhutM3A+zO8SW3RW5RGP0AhcS3RL4wrfo5RPnMJssQt4lqRtSSYmc1NFmGxSXIAhd5WSD4U0AgM1OR0AfP.fr83wSLeppZlIlqxZ7VY8p2DYgSRuShnmDg5rKSA.2..h4dxapol52YIKYIMwL+os2d6m+HKgfq7Jux9lwLlwSQDM42+8e+SevAG7Mi04T05qu9N+S4TNkCo53vtp6t69e0hlpeOybZi+CSanj89yjm7jcNVet1ZqsR.vEZggit4AUc.XW0byMmNBcWaEBkKszR6eLVGCyLw7Eahi83QUa5TQXgSR+PDQOM.lEBkj92Bw9JoW1zl1zpYUqZUtb4x0WE.i71iG7rO6y9SRKsztnG4QdjBZs0VuuXb9TkGyJONmiGUPAE7ovZ5o4..mgEMOwLUchHO4IO4zGqO2zm9z+yVYrnY9uHhh4MLVhpIMoIMMUGCBwvbmwZ4MZlIlqx9uorAZzHCKI8mB.yn+96uH.7sQrUxQYmat4VMyrqN5niu.Fk5551tsaq6YNyYduOzC8PoUWc0cMCMzP1kxbom5qu9+IUGDwABBfeoEMWWpEMOFAFV2aX4yjc1YOpMDf1au8khn7vXINwcp5.vNKmbxIQsKcIzSN.vbhkAHtbEy6u+9OgM0eg5PDwYmc1cRD83DQS0sa2Gs6tDskrQ1ScpS8M84yWcu4a9lmLFkDz+9e+u+fKcoKsJGNbL40u90e9d73YewxWClMe97cA1w1vmNps1Z6YrnoxtkX4Gof4LmQ9A5omdxaZSaZFV++0F58Hhh4R8KA2xTc.HDivWOVdxlUh4Iahi8354dtmKOUM2hIDNqrx5.DQOJQTgc2c2yD.+DDEsAQGNbr3uvW3KrKud8to0st0MeL56L5fW3EdgaLiLxnr0st0s3AGbv5i0u.LAOrSmNk18oAYFyXFsB.q3T.VkktWz3uof4L+g+WXlWbN4jygAvXVhKI.tYUG.wAtRUG.BwHbmHF5NKlRxyu669tJcEqO0S8TKUkyuHpvETPAsRD8aAPV81aukgPaHpIT8v5zoykshUrhc3ymuOYyadyKAi9+3fujK4R1RZok1B27l27hBFL3mZ.wuQ3..3Gp5fHNCCfepELOIYy5o4ulBlyhFwe+mofXPmzD.1tpCBatj.vBUcPHDiPlgOPFiJlRh4olZpJMw7ku7kuBUN+hXCQTvbyM28RD8ipolZb1d6suDLAWgOGNbrvy5rNqZYlac+6e+WLN9MIJ..urksrslbxIO+1au8OOh9xownrLhHYiKavZu81sjUGtiN5Hkw+QoM1Er91J6LGwe+Br34W27MIhXUGD1Y8zSOGW4QIDZhntKSYJIlO0oNUU+KnlY3i1YgMWEUTwPEWbweDQzpt268dSukVZ4RCFL3DoTOldokV5ZYlc2c2ceSqd0qdz1.ZbwEW7lBWy62nQE6SP2BQTiJZtiqUbwEeH.rSKXpFy1AntIbmY4ygI3cjJFM+gGBXDk1RBl9.vaq5fvtKiLxnLUGCBwX3eNZehlRh4EWbwV9N9eX9jibjibJ.nXEFCBSv8ce2m2RJoj0lbxIe5UWc0St0Va8VPjefB4L+7y+O+k+xeY2CN3fO3V25VG4sUG.HPVYk0S9pu5qly.CLvZMvPe77ZDQIxsKNqvsa1Svoe5mtsZw.HhZafAF3brvo7LO5engFZv17lXLI2HQjz8vhQNb3XkpNFDhwvRXl0qFQhWudeWVAZt4lk5MKwB8BuvKL2N6ryGNJtb4cZqs19bXzeCpzZVyZpr2d60vuFcDNBauNbZrkZngFRynegyue+6hYNMl41Xl48rm8Xa5NDLy+Nl4uIy7YZzee4DAg+2Z9746rsx4UyDfYd7NETEiCl4j3P+7SgPWM6n4ZaSqyoLv.CnjM0R0UW8dUw7JTF9ptpqZeEVXg2V0UWsy8su8cgACF7iivm64N8oO85XlOR+82+M85u9qO7U7j+ReouTU4jSNetFarQSHrCwue+UHGE2luRKsTu.3wMxwLkTRoyvu1c4..Se5S+LGmmhNYV.3o.v6akS5gNzglFyrSGNb77V47pY9QDQ9UcPXmwLSABD3qCfrUcrHDm.mtpCfiAGZEYrT986mQLzhZDwO1vF1PA97461Yl8NQtFZngF5c15V25Jti63NxFguVZfAFXylzkr0B45UKCy7bL3W+ZN73lDyrKl4sp5uFiTLyWuA+8BQDpiN5HSU+5ucTs0VqCl4alYdWp90PgHB8qT8+t4XvLe+V82AZu816R0ecKzKUUUUI2SO8rTl4ZlnWO0byM6acqaccXBWpxLyra2tU4oiaBGlYhYd+F4qgH7arhY9GxLyu669t1hdxMybYF42GDQrGT0u1aCQ9746bYl6R0u3IDSD986+eKZtfWYGBPlggFZHs9DcTX8prxJCjat4VKQzEzRKsLYDZS.FQ2F4YNyY5XEqXEQcuHc7jQFYrEyZrEGuvsltawHGyitIFOxQNxZ..Niy3LNeib7MKG3.GncUGCIh5qu9teUGC1HI4ymuymYtAGNb7N.n.UGPBwDQJojRuQyyyLSL2x2bKojRJcX0yov9njRJ4vDQO..x.g5gxVQKzar3mHRkcunDUqG.F122m8rmc9..0We8s..jRJo7exLq8GzPSaZSa.UGCIfdoIMoIo5yJAs2t28tcxLeMLy83vgiZPn8CgPXG0Rz7jLyDyGs9EsoJiLxHRacdhDXDQCQD8V.XAd85cN.nZEDFefBlyDdgO.m9tF3PNG.fksrkczj8KD.WqAN9lEF.5xIdaBAud8Jmpum.s1ZqSlY9dJqrx7BfmGxF6TX+MQNyU9LlYh4SxDG6QUlYlorJPhHFQDmd5o2.QTk81auSF.+NKZpa..WuEMWhi2yZfi04.7YGXOGsDo9Kr83.NaipN.Rf7IokVZMp5fP2vLmTe802BYlesvGDX2qpiIgv.EocHtigYlXdtl3XOpRIkTjRCPDUxM2bOLQzOdW6ZWSB.OhYLGtc6t5ZpolzIhlCQTClwbHFegKgn6vfFtgu53CeOC7eXPiuoIPf.aS0wPhB+98eSg2iCB.zbyMmNy7WC.ckUVYsU.bIpNlDBC1kQD4IZdhlYh4p31PImjZhXx7m+76iH515niNNUCdn22q7JuxWuhJpP5Y45gG0fFmEwLmR3+7OYXe7qhYdNFzbXJb61srY4sFG1gCG0o5fP0Xlot6t6YxL+nyblyb..7eAf7UcbIDFrWA.yhH50h1AHtpTVPn5lTHhYuy67N61fGxUUYkUJuwQMAQjabrIRG0ZpollR3wb8aZSaZ3+bu+ciX7MKtc69fpNFRPbSgK0oDRbn97+YCfON+7yuYXvcFIgPgFDgRD+63ymuSC.oSD8EIhZNVFTS6vMgCcXarPyZ7GC+dhHi5VTKRfEd0NMpUTrIhnRg7FG0JLyNAPeHF6fTtc69KlUVY8JCabWO.pH7es.hntikw2rzTSMMmRJoDSYUy6ryNwTlxT.Qx4mU80WuyxKubepNNrZLyIgPknxyBYkwE1SGB.0hPahycgP6Or1C+wcSDYJ+65TF+GRTSEqPPbUeYWnTWkQMPCMzP2Hjjx0NDQCxL+OfP2R8nVlYl4+.BspIG0MAf8F9O+UAviEKiuYYvAGrOidL84yGdhm3IPu81Ktq65tL5g2N59RPSJ+T.vaBfoq5XQHFEAAP8.ntCe3Cu8t5pq84ymuVSN4j6xmOe85vgiAJu7x8CE86sMyDy62DG6wh12+fE1BD.9oFzX0hCGNVuAMVBiW0.3gQrc3kbMHzl.MH..Qz9XleR.7OBfeC.dbnlEp3DxmOeQ0FS5DYm6bm3u9W+qXMqYMF8PaK0d6saJajbcFy7kgi8MpJDFBWtbcA4kWdmEB08dFqSYY2.Xqg+usiPKRRaHzpb2G.7Bf.57lw1LKkkWF.WtYM9ig+DQjQ1ihEIfXlmJ.LhCqJ+.XtDQQ0gLfvZvLuT.7gwxXTWc0M8ktzk9YWyDtLYN..x8PG5PW3TlxTzt2bVs0VqikrjkXnqlamc1I5s2dQYkUlQNr1UafH5Kn5fvJE9v0ZHUGGB6k8t289idsW609nJqrR+EVXgYAfogPmQDmBBcWW7BfmgH5+DHzFIFgVH1TZrwFwN24N4UtxUF.gZ.HZaB2QJyLw7GF.eOyZ7GCONQz21hmSQbFl4aAwdW63v.XFQa6RRXsXlqFgJ6jnR+82+0mc1YeL8Gcud8dpNc5rd.zC.JxrpGwnU3Z.V1PxlD+98uzTSM0DptwBybpv.OYcEw8BBfqgH5uo5.QmXl0jsJZEWNUvbJhiD9cheew533xkqJjjxsUtADCkaRVYk0cOxOVZok1N.vuGgNSG9SQenYNFwghjvf8RuzKskw+QEeI7a9bSpNNDZMOHzYExYBfzjjxOdlYh4Fc6lKRLV0bjPDoJBgNV0iZACF70xO+72pAEOBK.QT+HTshGsl+G+we7wcnp0XiM9yQnD9+GYl0wNSwNTc.DOJPf.2aBb6Q0puS4B8U8.3eE.qB.mJBc91jIQzsQD8gDQxBCLJLyM+4dG+GhgSVwbQr5qDqCvN24NuYiHPDVKhn+Ny7qCfUDMO+EtvEdk.3oG9GqzRK06PCMzOJ4jS9O.fqCg1no5jO..KR0AQbluSJojhV2C6MYeBBkP1BTcfHLU6Dg1aNaC.6G.s60q2NGbvA6oyN6z87l277g3f58VELyZLOKDZGvZkdchnK1hmSQbhvkwRmH15PGuEQzEXLQjvpwLmABs+.hl2jeeUWc04MxUJcG6XGSd9ye9GBg5J.EpScC.l4aC.Ojpii3DM0e+8elYmc1cp5.Q0XlKGgRXSXe8w.XiG4HGYKd85cud85s8Ce3Ce3O3C9.227MeyCAIoaSiYlXNgP+Bti616ZhdahnyyBmOQbDl4BAPLcZHN3fCdxokVZpnLtDFjXoKs3ymuqwoSmUMhOLwLGD.3PG5PSZJSYJV8BVLl7506k4zoSo01Y.5t6tyofBJ3HpNNzAg+8+uO.NCUGKhwja.rA.7NCMzPaq2d6sg8su80YSM0TeUVYkJqGdKLYLy2Mas9.U+0rv9hY9aDqW+wg9ERBaNl4eSTdMPfZqs1LFkwacLyra2tWhJ95Yrru8suEFiWyKB4kU8qk5lAGbvEn5WTDrWNzO64tYluLud8dxMzPC4xLalkwrHFYpIQvLeR.XOl4bLBeBQzoagymHNBy7N.v7iggXtDQ62nhGg5vgZkf0.fkGEO8miH5aLhw6zPnZu8WQD8Kh8HzX79u+6O0y7LOSinm8mnKehHWpNHzLDy7APLtY5EQjsAf0Bf22mOeepWudaukVZouDwSc13Al5QXe0UWcCl43OJR0hmOQbBNzdhHVRJ++VRJO9Q3VI3EiP0E9D0041s6ydDersAfWG.+XXx+b2IhAFXfdTcLDG3RkjxGUbyM278p5fHNhe.r1AGbv63PG5PWXiM1Xoexm7IYQDkDQzoQDcGDQ+MmNctsbxImtkjxsuL8a6te+9e1TRIkqyrmmv1CQz7rn4RDGgY97QnUHMZDD.4Dtk6IhivQ+o.qmVZokIWRIk7Y8xdl4rAP2CLv.mWlYl4lMrfL17Y0+tXhKXvf2exIm7OU0wgt50e8W+LtnK5hjRLchIne+9eyCcnCs1t5pqOr4lade0Vascee228MHj59VXD5niNpvBqmJUzhFEwAXleln8hN+98eIpN9ElGl4yHJuz3EwHV7Cl4OGybG0VasNTzWNGGNzdiPLA4ymu+ebnRdRLFZngFtRU+5jl6fLyOTu816kussss4t5Uu5LfErfohDbadyadRV3E4Rh4hILl4jigq49WTc7KLeLye4n4hC+98eb8Eel4JYl23F1vFzhMfEy7uLFt9OgTf.A1JL2yADaOlYhYtCU+Zkl4cXluVWtbM6W4UdE4bWQnLDyrKK5hdIwbwDFybIQ40aalkULKgAy7MEMWjzWe8UznLV+Pl4pgFr5XLyWZTd8ehp1ppppjSY5wAGJw7co5WrTrCxLemG4HGY9ae6aW1CbB8Ay78aQ+ifco5uVE1OLy2PzbwVO8zSdpN1EVKl4edTboRiLyGWoqvLeqLyWtJ95XDwwrilq+SP4ZyadySR0ulYWvL6jY9uq5WzTf6gYVtNQDUrpU6a0Vz7LjEMOh3Ke+n34bq4latRmXHwyuF.+elfOmYAfmhGQOtmH5QAvgXlS1nBtnzAT77aW3ot5pqry9rOa4PDJBQDMH.9p.3zAfU2k1TF+98+ZDQx0IhnhUkX9t..1xV1xO..IOv.CLCDp0+Xz7ZBioHN1t28tcBfEMAeZ8AfmvDBGgliHhAvOE.+tI3S85B+eib7dWhn.FQrEsHh7hPmBfhwl+lZpo4tzktzno8YlPiHhIh9D.bR986+7QBvaD7fGLlN.oEI3rjDyIh5E.A6omddDhnfYlYlsAf+ISXpjDyESHkTRIyYh9bFZngtVhH4tyjfJbx42I.9USvm5yvLWlIDRFgmQ0AfFy+gO7gm6rm8rkChoX.QTvTSM0MBfh83wy4CflTcLYVFZngT9dGQXeYUqXNCfeWEUTwvSl4oAvfF77zmAOdh3bNc57hlfOEOu3K9hupoDLBaivImeO.3GLAepeLybllPHESN7gOrbjxO576xkqSZxSdxsn5.IdAQTvLxHiMRDUJ.VJ.pW0wjPnSrxNJwCM7+R3ae5JM34nWCd7Dw+9tSvG+MWYkUpzROPnGBeK5+i.3Jl.OsLAvaxZV27olZp4iTcLngFbfAFXN4me9Mq5.INESDUG.VH.JC.ujhiGgPKXY+xAhn1GkO15AviYfSS2F3XIhyEt8UcxSjmyd1ydpxjBGgMEQzpAvIgH+N1cV.39LuHZh6ptpq5Pvb12O1Ud.vryLyLaU0AR7tvuA28RD8U.Pd.3aiDf5PWHFKJeUapt5p+t.XmFzvIIlKhXEWbwSeB9T9CyadyynK+JQb.hn8Afo.fmKBeJ+Ll4ywDCoIDhnf.3wUcbnI5C.yhHRRNzhQD0CQziSDMsVZokL.vZUcLIDVMkmXdkUVYfZpolyDFS8gKaEZQDK2bycwSjG+92+9+ilUrHr+HhFjH5a.fSEQ1hMTiN0qi84y2Sp5XPCbHDJo7tTcfjnqjRJwy68du2WA.AUcrLQ4vgCYyeJhZJOwb.fJpnh92yd1yoZ.C0wUtLBwIvDYie1zbm6bkM.lXbQDsS.r..Le.T8I3g5..uFzfS+S.fTSM0sp5XPwZB.kRDImOAZhy4bNGOd734dTcbHDVIsHwb.f4Mu40J.N2XbXZyHhEQBiKC.6IPf.WO.1zI5A1e+8+SQntKjPLtBW2r6hHpRhnz22912EBfWeTdnKiY9Zr53azDtEfNQaAjwKpC.mBQT+pNPDGq5pqt+hpiAgvJoMIlCD5v1Xe6ae2YLLDRelULQbS.3TRIkTd1N6ryK8D8.2zl1jzhDEQKumzIcRqmH5hqu95chPk5x2C.uBBsJsmlRiti0D8fSJdvKAfyNbmBSnY5niNjEbSHTIl4jXl+.N53T0wuvdpmd5IuSv0UcCMobCDByFy7YGk+7W6n6m0rVWo33MzPC8Fp9BkIhFarQsYicKreztefDQTP2tcOQ5Kv6zue+W..tXhHoiYHhJ4jSNW7I3S+vPJiEQBBhnMO3fCd6pNNr.qhH5tC2QZDZLWtb82TcLHDVEsKwb.frxJqN.vGNhObvAFXfq..WC.9WC+eWHQT4olZpuEQznU+lBwnhYddi3Ck+X8X862uzxtDITV+5W+in5XvDcX.bRDQRxd1DczQG0p5XPHrJon5.3D3rAvLNzgNzTJnfB5G.MkYlYdzZ.TNjWDwpEAfcOr+9XVGi974qAyObDB8wJW4JGjYdM.3Ko5Xwf83.31j6tp8RGczQyKbgKT0we6TFa..fApjDQAQkgPXIz1DyCe6EaN7+IDlpCdvCt8hJpnQ8y40q2Ar3vQHzA2IhORLOH.9M.3gkCMH6o.ABzqpigIHYOIIhZZYorHDVfOY3+kMtwM1D.7zPCMbE.X4.XaG8ykUVYUrEGaBgxQDsC.bGpNNhRd.v8iPc.mTIh9oRR41WqbkqzmpiAgPHDVLl4zF1elXluLl4ABuQ6i0druPXKwLe8po2VLwDHPf+du816Wtu95qPlYYEKiyvL+Qp9ZrHUSM0zxU82uD1WZaorHDVsg2GiIhX.7pbniL8YVc0UKkTkHgDQzyvLWC.1..lqhCmixM.dV.7h82e+aI6rytKXCO51ESHaB.KV0AQjfHRpFAQTSRLWHNABeZHJa9SQBMhnV.PYLymG.dRXsIn2I.dA.rd.rE.zJ.7F9MOKRbrcUG.Qp.ABHIlKhZRh4BgPHhDLQzawLWF.lN.p.gZesKE.S0.F+2F.qC.ezfCNXCd858.e7G+w8UQEULjAL1B6u8o5.HRkTRIkrpiAgPHDBQBJlYppppJ4Zqs1bN3AO34xLW6InDb6lY9g8506UzTSMM2Zqs1LfzEKDiCud8dple0gaLZngFtTU+8Kg8krh4BgPHhIgKqj..nW.rIl4yD.eAD57nX..rqd6s2O0kKWsUZok58DLTBwnpmd5o6wpk1paHhjbqDQM4hGgPHDFpvmCEuQ3+SHhYABDnOUGCQpjRJIGpNFD1WxFTPHDBgPn01zl1jc5zZ0opC.g8kjXtPHDBgPqUYkUF.gN3nzdIkTRoM9OJgXzIIlKDBgPHrCdGUG.QBIwbQrPRLWHDBgPXGrYUG.QhjRJoLTcLHrujDyEBgPHD1AaQ0APjPRLWDKjDyEBgPHD1A6V0APjH0TSMKUGCB6KIwbgPHDBgcP6pN.hDolZpYq5XPXeIIlKDBgPHrCNhpCfHgCGNxS0wfv9RRLWHDBgPn8HhFB.sn53X7jRJoHIlKhZRh4BgPHDB6hmW0APDXxpN.D1WRh4BgPHDB6hWS0APDnPUG.B6KR0AfPHDBgPDIXlK..co53Xb3mHJUUGDB6IIwbgPHDBgs.yLA.O.vopikSDhnj..q53PX+HkxhPHDBgvVfHhAvCn53X7D9MPHDSXxENBgPHDBaCl44Bf8p53XbjJQjeUGDB6GYEyEBgPHD1I6G.6S0Aw3HEUG.B6IIwbgPHDBgsQ3xYYUpNNFGZcMvKzWRh4BgPHDB6ls.fUq5f3DHCUG.B6IIwbgPHDBgsR3UM+qA.2pNVFC4n5.PXOIIlKDBgPHrcHh7.fyR0wwXXJpN.D1SRh4BgPHDBaIhn5AvMn53XTTjpC.g8jjXtPHDBgv1hH5oAv8o53XDlppC.g8jjXtPHDBgvViH59.vCq53XXllpC.g8jjXtPHDBgvtiIh99.39UcfDVgpN.D1SRh4BgPHDh3AL.9Y.31Tcf.oqrHhRRh4BgPHDh3BDQLQzi.fyF.AUXnvJbtE1XRh4BgPHDh3JDQuOB0xB2lhBgCnn4UXyIIlKDBgPHh6PDcX.b5PMsSwMof4THDBgPHDB8FybFLy+Xl4.r4qqFZngzT8WyBgPHDBgPnsXlS1iGOylY95Xl+HSHo7sxLmgp+5THDBgPHDBak29se675pqttYlYuFPR4uLybJp9qIgPHDBgPHrs1vF1PJG9vG9xYl6OZxHOPf.OHyrru8DBgPHDBgvHTas05fY9mMAyK+FYlIUG6BgPHDBgPD2gYdxLyO03jP9tYlKS0wpH9h7N7DBgPHDhQAybl.3L.vWD.KD.d.vlAvKQDsaHGjPBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPLb++0mbtckFOhda.....jTQNQjqBAlf" ],
									"embed" : 1,
									"forceaspect" : 1,
									"id" : "obj-5",
									"maxclass" : "fpic",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "jit_matrix" ],
									"patching_rect" : [ 38.5, 18.584905660377359, 100.0, 52.830188679245282 ],
									"pic" : "rave.png"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-23",
									"maxclass" : "live.dial",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "float" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 130.0, 249.0, 45.0, 48.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "Bias z2[1]",
											"parameter_mmax" : 3.0,
											"parameter_mmin" : -3.0,
											"parameter_modmode" : 4,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "Bias z3",
											"parameter_type" : 0,
											"parameter_unitstyle" : 1
										}

									}
,
									"varname" : "Guidance[3]"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-22",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 138.0, 334.0, 29.5, 22.0 ],
									"text" : "+~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-21",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 91.0, 334.0, 29.5, 22.0 ],
									"text" : "+~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 42.0, 334.0, 29.5, 22.0 ],
									"text" : "+~"
								}

							}
, 							{
								"box" : 								{
									"bubble" : 1,
									"bubbleside" : 2,
									"id" : "obj-19",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 35.0, 201.0, 141.0, 39.0 ],
									"text" : "Prior behavior steering"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-95",
									"maxclass" : "live.dial",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "float" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 83.0, 249.0, 45.0, 48.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "Bias z2",
											"parameter_mmax" : 3.0,
											"parameter_mmin" : -3.0,
											"parameter_modmode" : 4,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "Bias z2",
											"parameter_type" : 0,
											"parameter_unitstyle" : 1
										}

									}
,
									"varname" : "Guidance[1]"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-98",
									"maxclass" : "live.dial",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "float" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 34.0, 249.0, 45.0, 48.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "Bias z1",
											"parameter_mmax" : 3.0,
											"parameter_mmin" : -3.0,
											"parameter_modmode" : 3,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "Bias z1",
											"parameter_type" : 0,
											"parameter_unitstyle" : 1
										}

									}
,
									"varname" : "Guidance"
								}

							}
, 							{
								"box" : 								{
									"bubble" : 1,
									"bubbleside" : 2,
									"id" : "obj-15",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 385.0, 146.0, 141.0, 39.0 ],
									"text" : "Infinite latent trajectories"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 174.0, 38.0, 468.0, 33.0 ],
									"text" : "The prior model learned to continuously generate latent trajectories, which can be decoded to audio sounding similar to the original dataset infinitely."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "ezdac~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 199.0, 548.0, 45.0, 45.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"lastchannelcount" : 0,
									"maxclass" : "live.gain~",
									"numinlets" : 2,
									"numoutlets" : 5,
									"outlettype" : [ "signal", "signal", "", "float", "list" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 199.0, 439.0, 68.0, 92.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "live.gain~",
											"parameter_mmax" : 6.0,
											"parameter_mmin" : -70.0,
											"parameter_modmode" : 3,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "live.gain~",
											"parameter_type" : 0,
											"parameter_unitstyle" : 4
										}

									}
,
									"varname" : "live.gain~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 348.0, 249.0, 223.0, 112.0 ],
									"range" : [ -5.0, 5.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 8,
									"numoutlets" : 1,
									"outlettype" : [ "multichannelsignal" ],
									"patching_rect" : [ 348.0, 210.0, 223.0, 22.0 ],
									"text" : "mc.pack~ 8"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 18.0,
									"id" : "obj-37",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 173.0, 14.0, 455.0, 27.0 ],
									"text" : "RAVE - Prior (unconditional) generation"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 48.0,
									"id" : "obj-40",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 14.0, 14.0, 149.0, 62.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 8,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 199.0, 392.0, 121.0, 22.0 ],
									"text" : "nn~ wheel decode"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 91.0, 95.0, 70.0, 22.0 ],
									"text" : "loadmess 0"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 8,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 199.0, 132.0, 121.0, 22.0 ],
									"text" : "nn~ wheel prior"
								}

							}
, 							{
								"box" : 								{
									"attr" : "enable",
									"id" : "obj-8",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 174.0, 95.0, 150.0, 22.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"hidden" : 1,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"hidden" : 1,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"hidden" : 1,
									"midpoints" : [ 100.5, 87.4296875, 183.5, 87.4296875 ],
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 7 ],
									"midpoints" : [ 310.5, 191.04296875, 561.5, 191.04296875 ],
									"order" : 0,
									"source" : [ "obj-2", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 6 ],
									"midpoints" : [ 295.928571428571445, 190.40234375, 532.35714285714289, 190.40234375 ],
									"order" : 0,
									"source" : [ "obj-2", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 5 ],
									"midpoints" : [ 281.35714285714289, 190.44140625, 503.214285714285722, 190.44140625 ],
									"order" : 0,
									"source" : [ "obj-2", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 4 ],
									"midpoints" : [ 266.785714285714278, 190.99609375, 474.071428571428555, 190.99609375 ],
									"order" : 0,
									"source" : [ "obj-2", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 3 ],
									"midpoints" : [ 252.214285714285722, 191.0, 444.928571428571445, 191.0 ],
									"order" : 0,
									"source" : [ "obj-2", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 2 ],
									"midpoints" : [ 237.642857142857139, 190.37890625, 415.785714285714278, 190.37890625 ],
									"order" : 0,
									"source" : [ "obj-2", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 1 ],
									"midpoints" : [ 223.071428571428555, 190.7734375, 386.64285714285711, 190.7734375 ],
									"order" : 0,
									"source" : [ "obj-2", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"midpoints" : [ 208.5, 189.95703125, 357.5, 189.95703125 ],
									"order" : 0,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 0 ],
									"midpoints" : [ 208.5, 307.0, 51.5, 307.0 ],
									"order" : 1,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"midpoints" : [ 223.071428571428555, 307.0, 100.5, 307.0 ],
									"order" : 1,
									"source" : [ "obj-2", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 0 ],
									"midpoints" : [ 237.642857142857139, 307.0, 147.5, 307.0 ],
									"order" : 1,
									"source" : [ "obj-2", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 7 ],
									"order" : 1,
									"source" : [ "obj-2", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 6 ],
									"order" : 1,
									"source" : [ "obj-2", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 5 ],
									"order" : 1,
									"source" : [ "obj-2", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 4 ],
									"order" : 1,
									"source" : [ "obj-2", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 3 ],
									"order" : 1,
									"source" : [ "obj-2", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"midpoints" : [ 51.5, 379.0, 208.5, 379.0 ],
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 1 ],
									"midpoints" : [ 100.5, 379.0, 223.071428571428555, 379.0 ],
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 2 ],
									"midpoints" : [ 147.5, 379.0, 237.642857142857139, 379.0 ],
									"source" : [ "obj-22", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 1 ],
									"source" : [ "obj-23", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 1 ],
									"source" : [ "obj-4", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 1 ],
									"order" : 0,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 0 ],
									"order" : 1,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"midpoints" : [ 183.5, 127.0, 208.5, 127.0 ],
									"order" : 1,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"midpoints" : [ 183.5, 174.296875, 208.5, 174.296875 ],
									"order" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 1 ],
									"source" : [ "obj-95", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 1 ],
									"source" : [ "obj-98", 0 ]
								}

							}
 ],
						"originid" : "pat-311"
					}
,
					"patching_rect" : [ 350.0, 290.0, 49.0, 22.0 ],
					"saved_object_attributes" : 					{
						"globalpatchername" : ""
					}
,
					"text" : "p priors"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 9,
							"minor" : 0,
							"revision" : 0,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "box",
						"rect" : [ 425.0, 242.0, 1019.0, 664.0 ],
						"gridsize" : [ 15.0, 15.0 ],
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-45",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 460.0, 515.0, 34.0, 22.0 ],
									"text" : "*~ 0."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-34",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 256.0, 370.0, 29.5, 22.0 ],
									"text" : "*~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-33",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 55.0, 358.0, 29.5, 22.0 ],
									"text" : "*~"
								}

							}
, 							{
								"box" : 								{
									"basictuning" : 440,
									"data" : 									{
										"clips" : [ 											{
												"absolutepath" : "cherokee.aif",
												"filename" : "cherokee.aif",
												"filekind" : "audiofile",
												"id" : "u015009188",
												"loop" : 1,
												"content_state" : 												{
													"loop" : 1
												}

											}
 ]
									}
,
									"followglobaltempo" : 0,
									"formantcorrection" : 0,
									"id" : "obj-23",
									"maxclass" : "playlist~",
									"mode" : "basic",
									"numinlets" : 1,
									"numoutlets" : 5,
									"originallength" : [ 0.0, "ticks" ],
									"originaltempo" : 120.0,
									"outlettype" : [ "signal", "signal", "signal", "", "dictionary" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 863.0, 30.0, 150.0, 30.0 ],
									"pitchcorrection" : 0,
									"quality" : "basic",
									"saved_attribute_attributes" : 									{
										"candicane2" : 										{
											"expression" : ""
										}
,
										"candicane3" : 										{
											"expression" : ""
										}
,
										"candicane4" : 										{
											"expression" : ""
										}
,
										"candicane5" : 										{
											"expression" : ""
										}
,
										"candicane6" : 										{
											"expression" : ""
										}
,
										"candicane7" : 										{
											"expression" : ""
										}
,
										"candicane8" : 										{
											"expression" : ""
										}

									}
,
									"timestretch" : [ 0 ]
								}

							}
, 							{
								"box" : 								{
									"basictuning" : 440,
									"data" : 									{
										"clips" : [ 											{
												"absolutepath" : "jongly.aif",
												"filename" : "jongly.aif",
												"filekind" : "audiofile",
												"id" : "u882007839",
												"loop" : 1,
												"content_state" : 												{
													"loop" : 1
												}

											}
 ]
									}
,
									"followglobaltempo" : 0,
									"formantcorrection" : 0,
									"id" : "obj-21",
									"maxclass" : "playlist~",
									"mode" : "basic",
									"numinlets" : 1,
									"numoutlets" : 5,
									"originallength" : [ 0.0, "ticks" ],
									"originaltempo" : 120.0,
									"outlettype" : [ "signal", "signal", "signal", "", "dictionary" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 701.0, 30.0, 150.0, 30.0 ],
									"pitchcorrection" : 0,
									"quality" : "basic",
									"saved_attribute_attributes" : 									{
										"candicane2" : 										{
											"expression" : ""
										}
,
										"candicane3" : 										{
											"expression" : ""
										}
,
										"candicane4" : 										{
											"expression" : ""
										}
,
										"candicane5" : 										{
											"expression" : ""
										}
,
										"candicane6" : 										{
											"expression" : ""
										}
,
										"candicane7" : 										{
											"expression" : ""
										}
,
										"candicane8" : 										{
											"expression" : ""
										}

									}
,
									"timestretch" : [ 0 ]
								}

							}
, 							{
								"box" : 								{
									"bubble" : 1,
									"bubbleside" : 0,
									"id" : "obj-8",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 229.399998813867569, 421.0, 159.200002372264862, 66.0 ],
									"text" : "Try to cut some latent dimensions to alter the sound!"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 701.0, 78.5, 40.0, 22.0 ],
									"text" : "*~ 0.8"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-10",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 266.0, 623.0, 71.0, 22.0 ],
									"text" : "s rave_help"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-9",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 266.0, 583.0, 32.0, 22.0 ],
									"text" : "rave"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 266.0, 543.0, 40.0, 22.0 ],
									"text" : "active"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"hidden" : 1,
									"id" : "obj-3",
									"index" : 1,
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 266.0, 511.0, 30.0, 30.0 ]
								}

							}
, 							{
								"box" : 								{
									"autofit" : 1,
									"data" : [ 34467, "png", "IBkSG0fBZn....PCIgDQRA..BXN..D.hHX....PCOVB4....DLmPIQEBHf.B7g.YHB..f.PRDEDU3wI6cmGdUUcu2.+6JmLOGlSXJ.BU.wAFTApHzVopUoZqP6q1aq9p1Vakq1dauV8ZEeqcv5LspUpzK2ps0ZPsE05bM8JNPsQ.gvTfj.jDRNj44SNC466erOACgLbNmrO60dexuOOO77PNm8Yu9sGOqyZuV+V.BgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBGCktC.gPHDBgvgx0V25Vyb5Se5iOu7xax.X5.XV.XL.3tTJ0Qza3IDBgPHDBQLJRNJRd8jbWbvUjtiUgPHDBgPHh4TXgEFOIergnx38lacGyBgPHDBgPDSoolZJGRVRXTobRxMq63VHDBgPHDhXF974aEjLPXVobRxOiticgPHDBgPHb7B10U9MQPExII8RxDz81fPHDBgPHDNZc1Ym4SxphvJkSRdE5daPHDBgPHDBGKRFGIu4gQExII2FjzQsPHDBgPHDQllZpoQQxsOLqTtWRlot2VDBgPHDBgvwgjJR9kFlUHuGKP2aOBgPHDBgP33PxzI4aXRUJ+ao6sGgPHDBgPHbTnQqjeQznqmXF90jT5W4BgPHDBgPDpHYxjbylTExIIeURFmt2tDBgPHDBgvwfjKfjsYhUJe6jLdcucIDBgPHDBgi.Iimj+JSrB4jjkQxj081lPHDBgPHDNBjLORdXStR4tIY55daSHDBgPHDBaOZL.OuZStB4jjsPxr081mPHDBgPHD1dzX.d9RQgJk2QKszxXz81mPHDBgPHD1djbFjr9nPkx8d7ie7b081mPHDBgPHD1Zznqqb8QgJjSR5sxJqbx5daTHDBgPHDBaMRlHI2RTpR4Apt5pyW2aiBgPHDBgPXqQirth6nTkx4wO9wmot2FEBgPHDBgvVijWZzpB4jj0UWcyQ2aiBgPHDBgPXaQRWz7mvfNIM0TSKP2amBgPHDBgPXaQxzHYQQyJk2Zqs9Yz81oPHDBgPHD1VzHUH1RzrR4s2d6Wkt2NEibEutC.gPHzER5B.YCfo.fYBfY.fdl7P1K.JPoTspovSHD8BI+R.34ilkgOe99dokVZOWzrLDBgPHDFTs0VaSfjeUR9VgPim0FISW2AsPLRFIiij2WzqMxOgeE.T5d6UHDBgHl1S9jO43KojRtk.ABDIOB72GxWVKDZQwEWbhLz9QzCWuC.hS2auBgPHDwjnQqrsTRtqg62X62u+0n6sGgXjllat4QSxpFtW+FBZr7xKOYcu8JDBgPDShj4RxxLwu3NPQEUTp5d6RHFoviGOygjdMwqgGPM2byyR2auBgPHDwjHYFjLfY+k2ABD3Iz81lPLRfOe9tby952A455GT2auBgPHDwrH48Ds9R7Zpoloq6sOgHFlhj2Vz5529ya9luYV5diVH5MY.MIDhXFjLN.3A.IDkJh8AfyPoTcGkV+BwHRAu182Afq0BK1tUJkKKr7DhgjLBjEBQLi1au8wgnWkxA.lM.thn35WHFwgjI.f2BVakxA.1sEWdBwPRlfgDBQLizRKsy1BJlMSxLTJUGVPYIDwzHYp.nHX7idsZ+BMTlBwfRZwbgPDKY0VPYDG.90VP4HDwzH4nAP4PCUJuiN537UJ0yZ0kqPHDBwHEJZQoWsfzQK7IDwDd0W8UmuEe85Izd6s+kz81uPHDBQLsBKrvIXwe+9gKnfBjANlPDdR7e7O9G+jt6taK9x0S32n6c.BgPHDw75ryNuNM7k7eKcucKDNEj7J0v0n81gIoL15D1ZR5RTHDwBTjrF.LNqtfat4lGU1YmciVc4JDNMjzMzv0n8xjTJUUQyBfjiC.0qTp.QyxQD6RF7mBgvwysa2SGZ5K7yJqrdFHMxgPLnJnfBRD5sR4e6nckxC5o.vMYAkiPHDBg8DIeIc97w8506R089.gvNqyN67Fz3knEQiIvnnNR1HIOrUTVBgPHD1N0TSMiWiegeOZonhJJZNwFIDNVG+3GOWcdwYIkTxXspsUJULWHDBwHYj7OqyuzuW9Y5degPX2Td4kmLIqRWWT1YmcdiV41KIeMRdyVYYJDBgPXKzPCMLEc8E9CfIq68IBgcQwEWbhjbuZ75w8CKdrzQxwQxPMMppHo7j1DBgPDSPQxsowuzu+rKZQ8kUgvNijIQxh04Eic1Ym4q68CCjRJojjnwXioJ4dFBgPHb75niNVpN+R+AgLqBJFQijIRiejpNcuV31qhFyDvCU1YRUTQEkU80W+WkjszSf1UWccNVQbJDBgPDUTXgEFOMFjU1QAHY55dejPnCETPAtH46q4qA6fjIZUayj75CVt+M1qIvHRFGIuPR9mHY8CR79mspXUX+I4dWgP33Px+S.7K0cbLHdckRcI.f5NPDBqBM5RFaF.59oF84TJ0e2JJHRlL.5rWuTo0We8WdYkUVaKZQK5uAf4EJqmCdvCl7rl0r5JpDjVDRp17l2bbyYNywUN4jS7olZpI5xkqDxHiLRD.I.fDAP7.nm9fe2c0UWd6niN5pyN6rC.zw68duWWqYMqYD8jyjTwbgP3nzRKsLlLxHiZ0cbLTBDHvsFe7wudcGGBgUgj2G.9gZNL9mJkZwvj+QwjToTpSYcRxqC.+2lPQbIJk50Lg0iYSUbwEmvDlvDRNkTRIyTSM0QCiIJpI.fICfY.fOE.lM.FkIUl9.v6.feO.dFkR42jVuBgPHLYJR9NQim8czPmc14M.oAPDi.zUWcc084zeuj7vV80bs0VaSvr21H4XH4y22WOX21oCSJzqBZ3dEjzka2tSmjSjjKfjeIRdmj7uPxpMossgika06SDBgPDhH4mU2eKQ3JPf.E41s6uKIuZRNaF5oRMgvQvsa2mUeNseyjL4N5niulEe41Ovr21BV46CSxGquuGI+blYvWYkUtDyN9CFmwQxzIY9jb4j76Rx+.IcalwezPQEUzLhF6SDBgPLLQizulY05T5T.RdKjLIcuOUHFtN5QO5nnQqi2iakjphJpnDHYaV30UGlQgezKI+AAW+WZed83n4OwIUeAETPjNnUiam6bmo0QGcLYRtXZLfT+sTCO0Byz5V25RsW6yM8mFhcj7HVEBgi.MZwpuitiCSTmvXPp895NPDhHQgEVX7Ke4KuD.LM..+98upDRHgWB.fjOD.9dVX3LckRUtYtBOxQNRNSYJSoA.iw1RVYkU887dczQGWXJojx+vLKuf1Qas012nxJq7vABDnKOd7vLyLy3750aBwGe7ImUVYkQlYl4XRLwDmb7wG+mB.KD.KC.iIJDKZWKszxrxJqrNzN24Nm4YcVm0A.vBUJ0Go63RHDhQz73wyrzbC2DMcajTZjDgSihFoAPRR1QGcbhtgAIOOK9ZnebzXCjFc2CRxZwI2PltHYsV71XTke+9YCMzv.99c0UW7UdkWgqcsqkKZQKh.fe8u9WeP+LQIApppphI+QH8PlsoDBgsFIckTRI8F5NNhhtW.bM5NHDhvAI+F.3+C.vgNzgVPpol56G70yF.a0BCki.fetYuRO3AO3jwmbc4u.8JKuzd6s+cPLTKT62ueb228ciy8bOWzUW8eFart5pCaaaaCYkUV3Zu1qEu8a+13QezGE4jSNVbzh3xKu7189129xvpKXgPHD.fjeKqtIYzgppppTG58FBg9Qxo0y4sEUTQyqWud7jrDK9RmoDE19TABD3s6o.NxQNxb648d+2+8mnEu8E0UYkUR.vUrhUPOd7n6vIT0Q6s29U0RKsLaRtHRlfYedfPHDh9nwFaLace2eKzmQ26uEhgRwEWbhLX17nt5pa185sTzHarXkt0nw13wN1wlSuKjst0slC.v5W+5Shl+.9zVnlZpgs0lUNVcMccPx4GMNevpI8qQgPXaQx+B.tBcGGVjGUoTqU2AgPLXBDHvFiKt3tN.LakRUROuNIWG.taKLT1M.NakR0sIudUjbOvXByoGSc+6e+Md5m9ouU.bVlb40u1291G18t2MhO93wjlzjvjlzjPd4kmUTzNcWqRo985NHDBgHlCImmVa+Eqmse1LULx19129VHMRMh426WmjWiUewRyM27nM6sORlAIuIqdaou74yGm27lGgQ+Zm.fuvK7B5NrbRtQy9bCqjzh4Bgv1gjwAfx.vT0crXwRPl9oE1QASMhGB.mmRob2yq60q2KHgDR3cr3vY0Jk54FtqjBKrv3W7hW7zRJojtb.7e..aSSRWYkUhm+4edzRKsfq7JuRbFmwYn6PxoYUJk5kzcPDIjJlKDBaGRtF.7r5NNzfbTJUS5NHDh9hjmE.JSoTs1yq0ZqsN2zSO8hs3P4UTJ0kgdkkTBCplZpobxJqrVB.tI.boC0GP3nc5Jk5.5NHBWREyihH4rfwDrPdvXxD43vH0NcD.TA.b2TSM0TokVZ6KbgKzOhrazHDwTHYx.nE.LRbT1eZJkpTcGDBwPozRKcJSe5SubXsoc4tN1wN1nl3DmXGg5GnjRJIoYNyY9o.vpAvs..IM6MxQq.XBJkJjOewNHdcG.wXTEUTQwWe80myV25VWI.98XHtoU1YmMVvBV.HI.vA.vNAv9CDHvQZngFpo1ZqsgxKu7VppppZuyN6rqcu6c6ykKWAN2y8b695u9qGv3XXhM1XiIWas05ZRSZRdSM0TaA.spTJoh9Bmn+KLxrR4..iC.REyE1Z+y+4+bzSe5SuDXwyEJczQGWXHTobUKszxnyHiLVN.99.XwQ+HSXSkA.ddRdoNo5CIsXdDfjSE.O..VB.FKrmUhnK.7S.vioTpl0cvHDgBRNZ.TmtiCM5qpTpQhcgGgCQwEWb5yctysbXwSvNc2c2OoKWt9l826EbLoLM.bU.3GAfrsxXSX6sHkRUjtChPkLyeFYdU.bUtc69sJojR9r0We8SB.YBfTN7gObJaYKaIym4YdlId3Ce3KE.GD.34dtm6eSoTiQoTIpTp3T8BLNNDO.xp81a+RgQ2dAvnkyVB.9+EAwXR.3mAflH4+IIkmNhvIXi5N.zrQZC1UgCRQEUTByctyc6v5m0Kace6ae2bueARFGImEIuO.3A.GBFyhtgbkx6pqtvK+xuLtm64dvu6286P6s2t4F0B6hmR2AP3PZw7H.I8FHPfuV7wGeAgvhGGIuHkR85g55+G9C+g48K+k+xpZokVFU1Ymci..M1Ximc1Ym8Nh3f1n+suBkRs2AagH4jgwM1pDFcGlnVFhfjw0d6sOt8u+82xBW3BcT8ALg4ijyD.kLjKXrsmPoT2jtCBgnenH4KBfKypK35pqtycricr+qd9aRNQ.7ygQCh0I.pG.tAPU.nF.TqGOdZp0Vas85pqNu986u6wO9wmznG8nWlKWtNwSj5sdq2BWzEcQ..X7ie73i+3OFie7i2B2xDVn3UJU.cGDgBoh4Q.RVF.VbuSYTQgx3dZqs1dzLxHiSTFM1Xi4mc1YueXzZ3QpG+fG7fe+YMqY00fT1ICfKA.OH.FS2c2cg0TSM+yxKu78r28t2ibvCdvie+2+82DLZkhHdxcHXq3uS.LW.7d.3KHc6lQlHoB.eL.l2Psrw3JToTxL.pv1gj+m.3WZ0ka2c28y4xkqUaFqqBJn.Wqd0q9DM1TSM0DdwW7EQf.Av4e9mOl8rm8f8wENTd858qjTRIEJMjpvohjepH3ybN6bm6Lsd96BKrv3I4ESiIyfafje1JpnhYtwMtwLJrvBi2qWuKlj0uicriI1m0SljbKkUVYqY8qe8yo3hKdUj7ODBIb+1H4ySx6fjWdv9j2PEypVZokwRx0Px2d.VuEEbaHhZlAR5hj+ffqqAs07EwtH4RCgygGInJcerPH5KRNaMdMgojEUV+5WeRae6aOT9tRQLl1ZqsGxLNGRDCokVZYL8bBRf.A90s1ZqeNR9tgv4SESxuQnTFznBteJZTw6p6YE7AevGbsabiazTtwFM5SeItoMsorevG7Au99Dq+jgy51sa2oSRo4JFAJ34ttGV24MFhtOdHD8EIOrNtVvue+W+vM1apolxgjeaZLikJF45rLiqErBRWYYXpnhJJ0ErfErRkR8W68qSiGM+x.vlfwnEu21C.1ALROhGF.UCfZangFZrs1Zqst5pqNl0rlkWLLxq4znahnTJkuHccDBkwjgw11Ahzt0CIyQoTM1mWak.3BTJ0O1DBSgMGI+R.340cbXi3RoTQbWDSHLSG9vGN2oN0odLMTzspTpb.PX2ufC98xeA.bO.HreB2hXRcCfopTpJ0cfHhhHohjuOIquOudbjbiA+UZt20t10pu0a8VyE.IB4GCo.fqu427alUM0TyWijdapolxo2KPwEWbhjbS88CVZokNdZzsZtaRt5BJn.WVVTKhJHYBjziE1pINA1wzupXDHZ7cbulNtHngFZ3xC23szRKMKR9azQ7JbD7TSM0Hit2XY7S5Wr6pWuVbj7UC95EWTQEMh7KYI47I4uijkPxNHYfA4hk8hgH0cRxo1OeteqEs4HhRH4ZMga1FS4nG8non6iKBA.fWudu.cccPwEWbh8SHo3.7CWI4WRWwpvQoERldT9RGglnnQ+tyMIOMfSz5B+tdcBfo7HzHYNj7JapollgSnUhI44GAWr7yFp069129xOPf.8t0U8B4IP3XQxTinaqFiq1ZqUlxvEZGISgFMphVf9bu8fOI0szeM1EIWfthSgiz6PitarHVBMlXClcedsKrWG3qGlTkFau81mTuVuAH42k1nILHRN1d8mJRdzH4Jklat4qECw9rm4Ydlkz6OSKszRDMfQI4nH4vIsSJFlH48FImmDqq4ladz59XiXjMZzHSujNuN3we7GeU.v0q7JuRRjbEjrdZjxF6u38xzYrJbjlrEeYkHZijaxiGOmTkB862+960AcSaRB4odpmJs94jJ2s1Zqiqehq6jjUQxkOXqyppppToQVbYYz3lZWYv+cYj7yPxyskVZY1UVYkSt3hKdTEWbwou90u9jfwLTpqf+KocsqccljjG3.GXh..ETPASa3bkRf.Ad61au8qhF8k76kjuAI+h8JzSs6t69j1OTd4kmb3tOkjWRvO+OijoMzeBgYJXlRPzOZqs1xU2GeDirQxq1JNWurxJiUWc08660c2cy9budxA3d0znKj9FQ4vUDaYwV80UhHje+9WGMxK3CVeCOgfGXCTVYkcaG9vG9y3ymuGo2Gwas0VMsAXve+u+2WV+cVUf.ApcCaXCmTbRxqsWKR+FCL5zW7bSxQSxsDEV2mXVA8ke4Wd58y6+gEVXgg8SPfF4e8dbsjz12MghUPxeeT37jXE4q6iOhQtH4jshSxqolZ30bMWC85Mjyhgqavh6Vas0wEMiWQrkN6ryYXUWSILAG9vGNWRNpA58OxQNxP0e1ZDlT2Xwsa2yfCdVq3V66mo3hKdBzHelewCz5kjw4ymuqvjNGOp68e+2ehG8nGMERtyAZQ56ORITPxoPxsGbcTFImd3tNDgm1au87rhyYbvNcceLRLxDMxRR8eSXahZs0V40bMWCeoW5T6sLM1XirwFa7Tdc2tcOnCXOZzp4BQnx1zcfEl.R9aGhC3+bSpbVxPTNjF+HfHVWc007F9meaar2hKt3vdzVSRUas0Vu6ihqyILHacp3mj0hD8uyV2GiDi7Pi9U9lrhSverG6w3JW4JYas01I856e+6mKZQKhUTQEmxGY.h4QSxqhjq1qWueKqH1EwDdBq95KQDfF2T5aQxkLDKWbLXp+6se62lUVYk82A8kGEhs3c61c5c1Ym4SxURx+SRd+zDlMqH4WM5ecfkokJpnhYFI6GJqrxlJ+jz5XIs0VaSX3tuUbxZpollgFO2vo3708wIwHOj7hspSv2xV1BO1wN1I8Z6YO6gie7imqXEqf986+jduicriM09DqIwSNCnIDgiIpqqyDgAR9sau81uxPX4lHIYSM0Dm27lGesW6Tm6EhzLEhtPiJ9+mowj8yL.fxiGOyxJuJwr40q2uKIGzbi9.ruX7jr2Ch2uDkzpjYQQxsoiyGbXVttOPIFY4nG8niRmmvWWc0wktzkR.vMu4MeJuO507bQvua5T6qKBQn4OouqzDgLRlII4C9fO3PNwdvdMZ0+m+y+IqolZNki5s0VaWpUD2lIZ7j.NwuhrnhJJWK5hjno82TSME18YbZru3p4mz54C0fAVDBZs0Vik51TQSedcerRLhRbjbWQ6SpCDHv1dm24ctee97cRudWc0EW6ZWKA.WzhVDat4lOo2ugFZnMZz3QISiY8YgHR4kRVXyYfFCTxPsh4gxzSbad858bcx8SYRFOI+SQuqOrTuQmc1Y9Qv9fjI4sEbcTFIk7KcjSwS9IQHFXWgtOXIF4vue++3n8Izd858sfQJ1My5qu9S5813F2HA.A.25V25.sJpMZGihQDVlduZSDxH4iPRt28t2AM+ASRWAO31Q0UW8JBgSB7xgnOq2m0uhjmCIuNRtJZRsRKImWjttJojRRpyN6bFj7KRiA85fkgXr698jLRx44YPxeEMZA8EFI6GGoq4laNRlMXGo5qn6iWhQFpu95maz9jYud8tcXL+W..jjGOexWgr28t2STo7m3IdhncnHFY61z2UZhvFIUG5PGZbbHxi0zn+GylZpoUF7uGJ0Sxq0R1HFDAiim1jVWw40qWm7zebGjbAQx1ds0Vadj7CI40SoemGxnQWCJhlMXGg5eS2GyDw9Ju7xSljsDMOQNPf.tW25V2IZLjG4Qdjw2y60c2cy0t10xktzkx268dunYXHD2GLoTXsUvwDn1Aj7BAveUoTi13OY282x0TSMcckVZo+wEtvE5yZiv9GIud.LOkRcJ477fueb.Hoie7i6pgFZPEHP.1RKsDHyLyLPs0Va20VasbricrJ.DuKWtxXFyXFe57xKuWvue+nqt5BoklirKa8CUJ0CBiVqIbnpt5p+BSXBS3SCfaWoTg6meDmVas0OS5om9eW2wgCxMpTpMp6fPDSSQxB.vUEMKjW7EewI7E+heQ2872kWd42Z94m+CC.31sa7Vu0agq3JtBm52gHb.BDHv2zkKWaT9tZGNRNyVZokw1Ou9swfs.dwEWbh88mjEHP.OaYKa4zr7.dXhjShjqijs02soASiM1HKnfBFr9Enc2KwHrK9rssssLI4syHHyuLRRAETfKJ8Ozv02U2G2Dw174y2ph1mDu28t2Kp2k4i9nOZ57SFP8riN5HZGBhQ11ejL1xrCjVLueTTQEkvBVvBJE.qRoT6rmWmjuVc0U2pG6XGaq0UWcSZzidzUzqOluO3C9fosjkrjpr9H1znH4XAvhAv+W.rpHcEcvCdP7ge3Ghq4ZtFSK3hRN..VfRoZOB9rwQiAp2eQ9038OOd7b4IkTRuntiCGl+CkR8P5NHDwlZpolFUVYkU8QyxnyN6b8olZp89IzFmOe9d63iO9KLZVtwn1cWc00d8506wRHgD5LgDRvqKWt.L529ICfD.P2.fd85MfWud84ymOuIlXhdiO936hjdAf+f+qGwmbxImD.RE.4.fQCf7.vrAvIky3cfp..WiRodWD9OQbgcla2tm.IKo2uFI+c85+ei84WmMOqOJitHoq1ZqsbI4kSxmlFCl0PRf.A32467c3QNxQLie4azlaRlUDtORwHX.kNRPvVKOp1GViQICRIQTAMFuG6LJe9aYEVXgwC.TSM0L98su8s.+98+9Q4xLVRQMzPCWPgEVX5PCMdJIUEVXgwSxbH4oWVYk8u42u+2Py6SBE+CRd1TF+Ww1H4mtO+cpA+uwwdMAG30q2aVCgmNnZpolxgjKijOHGhJcczidTVUUUE8tLzb0HIGit2AGKoqt5ZM59fZOJszR0cHDNtScerSDahj+fn8IuEUTQ4Bbht6oSNKdY07TWc0c9vl1SFHYR9746yRR6zWp2AI+9jbT5d+iPy74y2U0qSLND50rR1HLpxKu7r84y2mgjah8p+C5P0QEUTwjz8N0XAEUTQIPiaZpcG6XGqs29se66S2wQX3t08wOQrGRNin8ItM0TSeqdJuFZnguQzt7hgT+N1wNxVmmeDpnwSc4yP8kosBPxGxiGOyjxX7RPRUYkU1Y0mSRVrtiK6BRF2QO5Qmne+9ud5bmLYB7lu4adN5deoSme+9+F59.YOV+5W+J+m+y+4UOzKoswOS2G+DwVHYBznK6E0DHPfsiOoQpNomp7fo3hKN89KYJLRxt10tB6YnZa.EImIIeLKXWTUj7+fjSmRkwE8fjeU1OYsDm7r6Yz1q+5udZMzPCKijOuEbgqo5sdq25JCmsURlHiv9odrlfeIaHOdDhlppppNL.ha26d22jtikvv8p6ighXKj72DsOo8sdq2Z78Tdae6auuMf0.4u..TbwEOpnZvYi0Ymc9t56LCyQvwSzoSx6lFyaJCGtI4VH42kFS3hRtzT7IH4Dqqt5lzANvAl3fbRjsr+fY2rgMrgDpnhJNq.ABzyLnosWUUU08Eh+vq33mjR.+Oh56Ls4762eeGXzZy5W+5W..vgNzgVmtikvfTwbgogjmWz9D1JqrxueuJRUf.AdmP7iN4fw3OKBJ1NnwD91lH4cPxuFIWIIWPmc14o41s6ITd4kmcQEUTpEWbwIFb.oFGMZwUUu9WbEUTQo51s6uQDDCCaG5PG5KY8mUD8PRUs0VaFjbAj7lI4aw9+67eLRtflZpobJpnhR.1z9VuvBwgnB0jb4gxEUd734pnLnACKETPAt18t28r84y2CM.WvZmreRl+fs8TUUUMld+AZs0VGmEsqz14UdkWIIZSNlVQEUrcD7l8tc6Np2hgln6SyGFQAETfK2tcmNIyljigFYngj4PL6HKrWHYJLJOVO750a0.3DmWricL3QNpL...H.jDQAQki4FhezeWvXLyA38K1iGO+pJqrxqaO6YOKqvBK7zdgW3EFcvYRznQ2YP0c2caR6UBcu4a9lyJJrsXqPirWVBjLMRlNIiW2wjvFhjW5f7dwyvazjewVYrGKofBJvUIkTxbBDHvSX52wybsxAZafjSgjrvBKL4foYyQroOQ2tcey58vzmXCaXCmdOwUKszxKo63ILb+Z33V5jbwj7g3P2WjKlj2IMdLyIZ0wpHzPiJCskn5Ypj7sdq2Z98pXiyue+gx3KJ.ClkyH4kQxMSitK5LnQWWPKsdJ0PVj4W+q+0iVGaqBgsCI24f7dqNbtvp4ladlVYrGqZCaXCITZoktn.ABD0+xjHT+lq529129XIIKnfBFQWIk0st0krtO.0iJpnh+QuiMud8VrtiovfkL4BEregddj7sGlw6lI4mBxih1VgFU3Mpps1Z6cQuNtWYkUFpikiKSi6ZFPjb6Qy8W8mm5odJoOTKDz3QpPD7wgQxb548BNv0BqG8W4kW9H1VHMZ4EewWL0FZnguHIKw7uUXDqC1+sFtKRxidziNQKeGkMBMF071BO7C+v42mXyVj5FCQQ0JlSimH30PyeexgI4E.oB5ZGIyhVPWJ64dtm6DMJUgEVX9g3G6sfM8bDZL3EsTaXCaHAcucKDZGIGOI41111tfMrgMj.I2EBNPPBDHvFCyqq1qt2dhkQR0t10tFuGOdtCZOxzG8aklH40SxwZ06erKnQeY0VnxJq7u0mvSo6XJL8fQwiSKk8S1kxjUBMZAcgdnH46FkOFSe97QDrB1268duYwPbV9st5pKSMu+Y.QxyM5tW6TAa5ORQHrTj7766EGc0UWaIPf.uUDbc00q6smQPbUc0UedjLTGw+QKinaY79CIucMeL4D9M+leSeO93R2wTXJZzGyUjzpmjk90TFjWVNRZI4r+t6taVXgEl9d1ydlICwTjmOe9r0YfDZL.msRMp6sYgvVfjlV5bqs1ZKWcu8LRzd26dGse+9ucpmL.RIAS8VmBR5Hl81LSjLUMbLneczidzB5a7Ym566gHSOqrPxerl1VJixzkskoolZxNmOvOQVRxNiFoeQqxeQ2auB6mQpyfRmmIsdZM8zSuFSZcIBCyYNyo93iO9ew+3e7ORps1Z6yAfRsvhelKaYKaas2d6mGIWBO4TH28Xgwgcwsp6.nGuvK7Bqsuu1nF0nbZCJ2tMyUVmc1Y9.3mXlqyvvz.PkjLOMU9ijnxJqrdEcGDCj5qu9UA.p63XnzbyM+Ssvh6uagkkPXeQya5h++T2aKhSPc7ie7YQRcjV71LIiuiN5XJjirlMXoMp0xqrxJ+88WLtksrkwq6XKL8yL4iQupt2fnw3CImgNZEQJ+98aIcgkHzin68OgpRJojjrv8Kmkt2dE1OiDawbE.LqAlzyZRqGwvGG23FWIJk5xO1wN1XAviZgk8UA.eojRJGA.X0qd0S1BKacy1LSm9TO0S886uWO8zSOcqNVFl7YVqnppppTAfcXdVHA.rCJ847nhlZpobb4x0eT2ww.olZp4+R2wPnZhSbhmgEUTcAfhsnxR3fLhqh4ETPAlUpIpK.TgIstDlnINwIVmRoV692+9yD.2kFBgOmFJSKW0UWcZPecQhSxQNxQ1vcbG2Q8826kQFYXayBDC.+l0JJu7xatl05xDLUXSNeIFiJqrx5U0cPLHtmbyM210cPDJH4mJ0TSsHqnrZrwF+iJkJfUTVBmkQbULO0TS0rRl+OtRoL09BpvbM6YO6VUJ08TSM0jNhR886VZoE7we7Gi.ANo6udyCzxSxKIZDG5vDlvD9g5NF5wy7LOysOPuWRIkjSavGZZsXN.VlIttLC2NImjtChXI986+Zf4MtohFdbcG.gAKq6oVTQE8AVUYIbVFwUw7oMsoMFyX83ymum2LVOhnubyM21UJ0c0XiMlM.9eLy0cpolJdlm4YvMdi2HN1wNVOu7Y89u+6mx.7Q1fcNO9Fpb61c5.Xc5NN..Ju7xeza+1u8ALsikZpoZJWyagLyVQ6yahqKyxF0c.DqnkVZYLtb45o0cbLH1G.bq6fHLXY4e+rxJqxspxR3rLhqh44kWdlR++srxJ6.87+I4YPiIsieqYrtEQGiZTipYkRccd73YF.3.C4GHDDe7wiezO5GgpppJbEWwUf8u+8C.f4Mu4cd..jbQjr2oHr0O5QO5ejYT15z3F23rM8s7srks7iGr2OszRyok24MyVL+SahqKyxmu81aWxRKCSjLtLxHi2P2wwPXMJkx1mIV5k1rpBZlybl0ZUkkPXqQxa1LFJ08dZzkjtH4BBNPqDN.jLNR9cMiyEHIqt5p4C7.O.OsS6zXM0TCYvYS15qu9ISxEzqxMdRdEZbSeXq5pqNMyZ+1vUYkU1PN802TSM8P5NNCSeay33DMtujckiIKcXWQxustOHNX762us3IpENH4iXg6hxW2auBgs.I+SlwUTXD3SaHVjGOdNcR5wLNmfzX1vqGG4HG4y41s6IPxCCGvDqQnhj2oYs+Z35du26MqgJd6pqt9K5NNCSlxrILIsxz9VXqnhJxrFH9i3Px7z8wu95e8u9W89Oao2MdkSAMwIevPv3081qPXKPxFMiqnbh2zQz+ZngFlpYbNQe06JoSx4q6sSyPUUUksIukWZokFRyPljrXcGqgoulYbrhjYn6MjASSM0zLLisyQZnwSBoDce7q2ZngFH.ne+9IIYmc14mU26mhDj7RsvcaRd8Wzuh4a0WRNUFrO9FrxzlxTl9UcUW0rLi0iP+F0nF0Q5t6t+Cl45ryN6D20ccWn81OQVByJyq5QM4kWd2htigd7xu7KGpyPeNsqU8ZRqmjLo0STQVYk0Yp6Xvg51.vL0cPzaomd53q7U9J87m6KkTR4s0Y7Do74yW+lxUiVEmEVVBGjX9JlqTpiBf02Vask6pV0pLsQb8nF0n9ajbzl05SnWMzPC+2l45KkTRAYmc13QdjGAjD.Xwzg+nKO5QOZJ.3mq63..nrxJ6WdK2xszRHrnwAiI2FmDy5KrSzjVOQKKV2AfSCIOM.XpyLrlgDRHArwMtQ3xkK.iIbMmz.97DJszRsrYs4JpnBIGlK5Ww7ULG.7HG4HOaZok1wlvDlvtMiUne+9+hJkJekRYk+5ZQTTKszhoer7e+e+eGqYMqo2437q0rKCqzjm7jWqtigd7DOwSDpUNwx9hVSTWlz5wtWwb6XFiw1hjI.f2S2ww.I3DragJkZu5NVhTc1Yml07bxPxiGOx7fhneMRnh43ge3G9RMy02G+wercdVVSDAF6XGqozEm5sDRHALyYNSDe7mXVHecjzQdMW4kWdx.3Wp63..3fG7f+h6+9u+VCkk8odpmxtW4z9iY0h418mTvB0c.3v7P.Xb5NHFBeCcG.CGiabiadVUY0RKsHULWzuhenWDmMRdAczQG2gYtNW3BWnoMkYKrGxHiLrhJIjR80Wed.nRKnrLU4me9eKcGC83Iexm7WDpKa5omdxQyXIJwr5i4186um.IcISK4CMRtHLHynv1DOmRopP2AwvQt4lqkkJaas0Vcjc2GQzmir06BUjbl.3cRMUSM8h6CNz9Omn+EbvAaISXNidzidoVQ4XlHYRvn05ztCdvC9yC0VKG.HqrxJ8nY7DkXVsXtcuh4.1+V0W6HYx.3+U2wQH36p6.X3n7xKO43hKtKvpJuUrhUH0iPzuhYqXNIiG.6HJrpcbs1oXv4wimoB.qZlHzT6VUVjqC1j6U7Nuy6bugyxmXhIZ5cQIKvHkVLG.vI9DMrLAazfmB.on6XYH7jJk535NHBUj7TF6I4me9KyJigBJn.qr3DNH1hurMJ4b.PzXfbX2mBjEgGUxIm7u0BKuKyBKqgMRlH.dLcGG.FsV9MbC2PH2Z4..YjQFNwLmjY0h4Ng6uKyVxCtKD.qV2AQH3Gn6.HTErR4KqOulB.+J8DQBwIyIbi6HUlQo06SDkVuBMvue++e.vEYgE4n5Iu56PbMvlbehvs0xA.RIkTr6CVt9iYMFVbBmmYYYACmFRlB.dMcGGgf+KkREJotT6hoF7e81XAfokNkCEqYMqwJKNgChs3KbiF73wSog5xRRzc2C8.jtrxJaBJkZmCq.SXWn5pqt9Ztb45Opgx1QbcWvzy1Fzcb..bfCbfvt0xA.xHiLlXzHdhxLqVLWpXty1uG17IIJXbt5Cp6fHLc8.XR8409xVcPTXgEZ6u9jFyxrYSxYSxUQx0Qx+FIcSRujrVZLyJWPv2aUd73YVUUUUxSBaXvIzGDiHacqa06EcQgVCgVRIkfbyMWjYlCdirOiYLCGSenSLvH4bBDHvK6xkqooix+i9nOJN.3DxDEqF1jAm269tuaX2Z4..ojRJ4axgRTmGOdFI0h4Yn6.vNhjmObFcgkqQoTlUd2OpqvBKLY.bG.3I640B9DLuKsET1.A2GjB.xE.mAL5pOWI.FpuibLA+2bQvyWSJojPd4kGHYc.39AvyCfxUJkjdHCQNhVtKRrjkrjqMTW1st0shLxXn+9g8t28NpgSLIzORd1.XO5pR4..KbgKz1OZ7CN3o2ntiCfHu0xA.RIkTNMyNdh1H4HozwpSLq4DUEbbc3DFKS0.iJc4Xje94+eE7+d187ZM0TSYAfIX0wRs0VqVp+EIiqgFZHKRdlj75I4VfwjZV6.3P.3uBfuOF5JkOTFCLl6KND.ZgjeaRJWuGBhIqXNI+JokVZgzLCXWc0Ept5pgRMzMtzrm8rqijNwASl3S7mzc.TXgEp6PHTbYvljIHhzVKG.HojRxR62nlARZVOMEmPKlKeQ8o59fy3IIrJmTqf9QezGsr7yO+6L3et.D75ijRJo4pi3YricrVQ8uT0UWcYRxyhjeKR9F.HPN4jSS.3igQiurJD8exnoAfeC.Zkj+bRJcgsAQLWEyCl6x+yg5xWRIkfYMqYExq+N6rSouS4PsgMrgD.vr0cbrhUrBcGBCpfYsfMo63..Xe6aeQbqkGj1dxHQpt6taoEyGghjmF.tEcGGgf2C.Eo6fHTQxIO+4O+dmK3iaSaZSYA.jRJo7uoovxz6JwkWd4I6wimYRxqljalj9G8nGcy.XmvHwUXkI5fAxsCf1BFiwb0A0LDSsSIX+j5kBmOyG7Aev+S94ment3s+xu7KervMtD1CW1kcYeFcGCAY2aIyOK.rE4+6MsoMEwsVNbn2eyDawbm.okyBJXkTbBcgE.fUqTJaeWxqWpF.ePuegEtvE9MqpppFC.zxrZb6s29vpUpKrvBiu1ZqMOR94I4iPx5yO+76LojRpD.7GAvUA688.+i.XajLZkA8D1AAG4vgrt5pq8BfrN5QO5Psntat4lOOoar3b85u9qmFI8DNmeDs7W+q+Ua6ioljwQiQbu1s28t2Pp6nMHRT2aCQhlZpobLoikmmt2VBA2tYrsFKfF82Wm.mVVXA.mHCi7Z5dmWOdoW5kB4LFEIi6nG8nixqWuKlj2NI2ulCeyjWRp8mjscRrVVY4ABmEd6ae6eU.3a7ie7C0hNurxJKIir3PUTQEkvBVvB9.XSR8X4kWdYCfgS2yHZZI.P649aRNbasbrwMtQaww6vU2gRtaMD3ymOjPB1hjpyfwVLNFzMRlErIC15gfO.7eMjKkMjRoBPxqB1j68lRJoLZ.TU+7Vp5pqtLF8nG8zAvRAvWA.Wvjm7jsz3yBk..1KIWgRo9G5NXrCryOlivBMlLFBmo678r3Eu3c+C+g+PWIlXhC1x8ycRS0vhS19129xXAKXAeL.lmtikdblm4YNecGC8G9IS+2Z292+9+o2+8e+Cqu.cricrNxwCRf.ALqJla26xT..Iq6.vl3+Q2APH5pTJkGcGDQBRp9W+q+UN..G+3GGEWbwZMdl6bm674ImgTtAR9pjr6f8K7c.fGE.WfVCTqSgj7Sq6fvNHloh4.XQgyBWc0U+M..ufK3BFzVUq4la1Q9X6Foq3hKNQe97cEm9oe50Cav.9r2RJojteZOm8OOaXSFrjaZSa59FtqijRJIaaWFZv30qWSoOl6wiG634X80H9JlSx4CfqP2wQH3iUJUXMFtrCHYJj7sAP2KZQK5n..MzPC3gdnGBj5qaxOgILgMgSNCo7j.3h0V.YOrURlutCBcKVph4eyvXYOdd4k21A.l+7m+f9nTSJojxjjNxVdajnfs7P0yctysq3iO9+BrISPN8wLAfsJ+ZG7GJXKxDK6YO6YX2Z4..omd51hAvZ3JPf.lRsE762uTwbaNZjAjdUcGGgnKC.NoA7YO9Y.3jREVYjQFXSaZSX+6e+ZJjDChcPRGY2PzrDSTw7BJn.W.3ZB0kuzRK8lPvavL1wN1AcDAmbxI+FJkpygWDJrPEBMLYQDAra8s1YAfyR2AAIwi+3O9uzLVWomd5iwLVOVMOd7XJckEOd73Dt+tc65.q1MBavX5HD7yUJUk5NHBWzHS27856qmat4hm7IexPZhETX4xF.+NcGD5TLwf+bQKZQgThHuwFaDG3.G.O8S+zmHkTkXhINTylmeA3LakfQb7506E..Gwry5G8QezQzcLzGaP2A..PwEW7O8we7GuMyXckRJo3D9AZmBud8ZJ2uoqt5xkYrdhxbBwXTQvA74uQ2wQHnc.7+S2AQjn4laN6rxJqS40iKt3vMbC2fFhHQH5ZH4uPoT6Q2AhNDSTw7ryN6PZBB3Mdi2.e0u5W8iAPu+h+ALkrzc2c+xtb45fC23SD0o762+Zb4xUHOwRoY6agKbgMq6fnGjbx.3BsAwAdhm3ILkVKG.HszRaRl05xJUas0ZJsXdGczgSnRuNgV0OZwIjEV..tLkR4U2AQjH0TS0VLlYDQjMCf4hQfMLZrvMEiK6ry9GDJK3jlzj.Ll8q5soNPKeM0TyH5GmhSPIkTxLH464fpTNf8al8yVL.mMyVKG.H0TSMeyZcYkps1ZMkuHpiN5XPS2T1DNge7foKXda9pzcbDB9..7+NjKkMUBIjvJ0cLHhXyljZu6UpCN9Jl2byMOCDhCvu4Mu4gK9hu380mWdlCzxmWd48WH4xFNwmv7QR0QO5QOMR91ybly7P.Xw5NlBCsBf2V2AQOH4X.vpsAwgo1Z4..ojRJ1pAXanZ0qd0l0f+zILvJiIdpsgif86YmR1M4K6vlgOOgfi8rertiCwvxSA6+LksoywWw7LyLyqOLVV7m+y+4b68q0VasclCwG62FQAlvTPREImVu96wBf+9jm7jOH5yHs2gX0JkxNMkqem5N..L+VKG.HwDSzVklLsZABDHccGCh90p.vLzcPDBdHkRUstChHj5Juxq7W.YvE6zMut5pq4p6fvp4nqXdvTM0OLTV1ibjifibjizZZokl6d+5s0Vam8P7Qkgssdc1d73g..j7R.vwgyrB4..6C.uwPtTVDRlNrAcqlnQqkC.UbwEmSHaWbJLqVnzmOeNxrRSrrfoAtBzcbDB5thJpvV7i1CWG9vGNWR9lwGe7gTcCD1aIlXhOGFg0p4N5JlCfOEFhsgN6rS7.OvCrh7yO+3yO+7yLgDRnvd81w0c2c2u+h5t6t6eoGOdlAbtUBLlP2c28C9ge3GFn1ZqMC.7J5NdFltDa1iE9aq6...XW6ZWldqkCm881LqtxR9lw5IJyutC.K1+ErmysBmj.ABb8SYJSwwjlfKpnhR0mOeeFRtsoN0odL.7Y0cLILMeJe97cQ5NHrRN8922sNXuoe+9wO4m7S97268du+iAXQRxkq9crGcr3hKtaOkTRwNUIpQbJnfBRLt3haEKaYK6n5NVLA+TkRYaRQhEWbwIB.ytUpCajD+1e6u0zii0st0Y6q7SzlOe9VttiAwmH334vIzmmaeqacq+AcGDCFRFWGczQtolZpKG.+G.3bzbHIhhhO93+qEWbwYeFmwY3HyNPgKGaqJQxDgwjyv.ZG6XG0bu2689lCxhjd7wep+1jibjirdaVKaNRjZAKXA+TcGDljZfMKO.O24N2uDrAW+GkZsbje946DF3i8Geu9q+5ogge1JIAud85DxnAijtOqsXl0cnbricr+sUrhUXqdRFjLtVas0wSxKijaF.ARM0TqD.+AHUJejfTl6bm6Cn6fvp3jaw7yenVfryN66.C9M9GcRIcpy7qG7fGrpgQbIF9b8tu669ql9zm92Q2AhIY4Jkx17EcAGaF1hbn769tuaToU6SJojRMZrds.IrxUtx1HI.P2vXbI7Q.XO.nX.TN.pB.sNXMdPiM13Wu+lXUrgrMWWDMEL8HdY5NNBAsNwINwWT2AAIiqs1ZaLomd5KD.eM.7URO8z0dCIHzp01UWc8emTRIsScGHQaN4SzuqA6M2111Fdxm7Iec.D292+9+4s2d6Wd+rXSJ0TO0u+9y849bCXJTTDc8XO1iM45pqtCrzktzXkJk+iUJ0AzcPzGW..RS2AA.v2869cakggPc8NpQMpLilwsEINXLAa70gQ2N5uAf8BflAPWj72RxyIX52C.mHKFciYmc1aTobDiWptzc.XAT.3un6fHTzVascS.PKYMp5pqtLI4xI4FAfuzSOc2v3b9+OvYWWEgIIwDS78c61sjsoriHYJC0Wfum8rGBf3eoW5ktmfuzozJ3qbkq7lFfOdsjzQ7sZwPR7fG7f+jvoRZN.6k8pRS1DJRdHcuiIREhaiwcvCdv6V2wpEp38rm8bdd858BHYw5NXBSw7oiVRdQ5dmbnp7xK2R6BXszRKikj2HIOpt21ENFai1uuW0T4T6JKCUJNDG4HGAKe4K+KuxUtxdR4S4AiVt3De49YbFmwBFfO9X.vn.P8C2.UL3JnfBbcVm0YspYMqY8L.3T6WQNaKSoTlxzqtYgjmNbF4P4HxS+zO8jV0pV0qkYlYNRJ22N24Lm4rMcGDQnX5VLmjwCfmW2wQH5+dZSaZdrhBhFyMEOC.NOqn7DwTNO.bc.HlclY2oVw7u9Ps.ae6aGemuy24OmXhexrR8JW4Jmwa7Fuwg54um1zl1fMqdNG.r0gUTJFLwsqcsqO6bm6b+SwEWbwh4a4KQoT0o6fnerdcG.QIwWVYk8il1zl18n6.QDVh0yxBWGbHyEFG+3G+WYEkCImOLF2DBQjZij7YTJUG5NPhFbbONffOBiAc19r4laF24cdmXIKYImzqe+2+8ePR1SqjG2TlxTFr9RdNCuHUze1vF1PBG7fGbUjr14Mu48FwnUJ+ITJ0qo6fnuZs0VGG.h4xGrEVXgmgOe9pRpTtijOcG.QKjLU3fl4nKszRKwhJpGwhJGQrsqQ2APzhSrEyGEFhIng2+8eer10tVLwINwS50Oyy7LA.tYXzJFIMgILgAbczVasU9vNREmvl1zlx9hu3K9aNgILgeAbf+fvvvA.vZ0cPzeRO8ziolI7JrvBSd9ye9+pLyLyAMsoJr0bLShMQf6V2AP3XIKYIVR2XA.KzhJGQrs6F.OotChnAmXEyGzbyqOe9vi7HOBtu669Nk2ijn95q+BC9midbiafmwtSO8zeGRNwX0GUhEItst0sNuy4bNm6NszR6JzcvXA7Afy2NkZD6AISF.+.cGGlDU4kW9Ele94+J.nem4dENFwjULmjiF.NoeHbSv5xo7aG.K0hJKQrK25N.hVbhUL+hGr27.G3.X4Ke48z53mj29sea32u+dpMd94latC1pJaXLfQOzfsPhSghj4AiGyzOAwdCnyAyBTJUS5NHF.eIcG.lgssssk4BW3BeVWtbMn2GP3XDSVwb.7D5N.BSV4fG9mBfW0BKOQro0o6.HZwI1kBt1A6Mm7jmL99e+uO5aN70iGO31tsa61xLyLON.vO5G8iVX+M4B0GZIet5zPRWjbFj7NHYq.nRXj2kGIUo7qUoT6V2AQ+I33x3wzcbXFNuy67ZVpTdLkXtJlSx7AvUo63HLYk8E92.FSTVBQjpADC+i6bTsXNISDFoxvAz.Ma20XiMhO5i9nGnqt55o..V7hWbnz0JZIrCxQHBlFvlG.9V.3Fgy7G4YVd..7T5NHFDyCFOAHgvtwp5WyVBZL+W7mzcbDldE.XYy1mJkpaRtD.TsUUlhXNWncrKiZVbZUlZTQ5GriN5..fqXEqnF.3ZQKZQW3P7QvAO3AaKRKuXUjbLj7dgQ+od6vnh4NsyiLSOG.tsAa5Q2F390c.HDCfXpJlCfyD.KV2AQ33C9fO3pTJkk9zgUJUMPFDnhHyMpTph0cPDM4zpP0DG5Eo+0c2cCDbvsbm24cNqd2+xOzgNDd5m9o66Gw2Lm4Li0ywtgLRFmOe9VE.pE.2ltiGahWG.eU61jHTuQxrQLXJRTDyHVZv0qfyYxD5DrvrwxIQoTeD.VxPtfBwm3tPL7DKTObZULOhmwByHiL.LtwI95e8u9C2yq62ueb0W8UC+9OkmJxqayaETqfpkVZYLj7pAPUwGe7aQ2AjMxqCfufU2RSQfUq6.PHFDwL8wbRtL3.mUcau81OWcU1Jk5C.v70U4KbT9d.3mNRndYNp9XN.lUj9AC12yUeuu22KuYNyY94640c4xEl9zmNF8nGce+H1tIHFyFIOW.7+EFsZ0G1VasUSiM1Xxomd5yHmbxY4.3KhgHmwOB0+Kb.UJOX+c8TyanBg8QLQEyIoKXzs1bbRM0T2FIALpzyO1pKekRsCRNG.rWqtrENFKWoT+u5NHrJNsJle5Q5GLkTRA21scayadyadmTBouqt5BiabiCSZRS5jV95qu9X5oLXR9M.v+SueszSOcjd5oqm.x4nb.rR6dkxCZ5PFzmB6stzc.XRtbLDIl.Gf+ktJXkRsuFZngyLmbxYW5JFD1ReL.9bJkpNcGHVImVWYYX8H2t7K+x2YhIl3h58qcW20cge8u9WioN0odRKaEUTQECmxxNhjpctycNt.AB7znOUJWDZZt4lWnRobJi8.Yr.Hr6b7C9yfYnJmVlX4TTSM0720Y4OpQMpcWas09EzYLHrM5F.ec.L+QZUJGbnOqfB...B.IQTPTwA0h4Aer7yb3rNV5RWJ74y2I8Zqd0qFkTRIXTi5jS3Ku268dMObJK6fdRogABDXItb45bAvpNqy5rjVPMB8pu5qtpK8RuzFzcbDJpt5pSCFowRgvNywWwb.70fyeFn84xM2baW2AAIKQ2wfP6dfJpnh6ZJSYJwDcysHgioh4.HUyXkjPBmbWld9ye93ge3G9TlPhBDHvIWCdGDRlXkUV44.fBAPJtb4R2gTrficIWxk72zcPDpF8nG8OP2wfPDBbzULmjIAqcx4Ipn0Va8tzY4WTQEk5BVvB9ovX.9IFY50polZ9F4latGW2Aht4XpXd0UW8T5cJNzr3xkKLsoMsS40862usta9D7KDlUWc00LiKt3lbBIjvTfQeJd4.H691m4ECaeA6bZQr21wN1wDSHgDtacGGBQHvo2GyuI37Gf70kYlYtecU3jLKXLSNZq+NWQTSms1ZqWRlYl46ffoz5Q5bLULulZp4RiFULefnTprgMJiADrq7nd0W8US3bO2ycF.Xm.HgjRZjzrdu17kUJ0N0cPDJJt3hSetyctEo63PHBQN4mLYp.3gGxEz9asPuUHpE.bq.3WowXPnGu3gO7g+JSaZSyQ+jyLapgdQzOR5xsa2MN9wO9Lrpx7C+vObMm24cda1pJO.isyZqs1wkPBIL6LxHi46xkqECfE.foNTeVQTQW.XwJkZG5NPBAJ2tceliabi6swvXFxUHrXonTJG4WJSx0Af6V2wwv0wN1wRahSbhZehdhjSFFyODyV2whH5ymOeqMwDS7wfzJ4mBaeEyC1Rw+FXL0uakJWoTy.VvIMAugzOA.WaztrDgr2qt5p6RF6XGaq5NPFDtdu268F8Ye1m8hSM0T+Y.Xt5NfDhvThJkxw0p4jLc.Xmu2Pn5YTJ0Uq6fnWhij2A.tGcGHhnmN5niqLszR6up63vtxITw7YC8MwCLakRYp88NRFG.heO6YOHszRKtbyM2omTRIsGyrLDCaWqRodJDF+nrf+.xD.PxA+WR.HQ.jPokVZ76ae6yUSM0jqFZnAUqs1pJkTRIN.fTSMUjXhIpRIkTPZoklJqrxRkTRI4JszRykKWthO93iOIWtbkbBIjPlomd5iJ8zSehIlXhmN.NeHsLtv4KdGxbBvIgj2G.9g5NNFt7506YlTRIsacGG8UGczwpSIkTJP2wgvz4C.WXvY7Uw.vITw7aG.+bMU7aUoTKa3tRZokVFSFYjw0.i9xmiaJadDjc.fOuRopMTV3fUF+7fQ9BeUPF7RB8xGLxpEEb3Ce3Ve1m8YW0sbK2xylbxIq63Z.oTp3fC6QYSxrAPi5NNLCadyaN90rl0XK+gQd85csIjPBR+Nef8B+q+0+5NJszR6X9ye9epYMqY83XXlRoixdN.bcJkpMcGH1cNgJluMXT4GsnyN6bpolZpGML9Htd4W9kybZSaZ4OyYNyKLgDR3lgTYbmfqE.Ocnl4UJt3hSbtyct+E.boQ0nRHFZcCfa3i9nO5OrvEtvSpagzZqstszSOcsc+yghpu4oVG.R9X.36n63vD7+nTpqS2Aw.ofBJv0pW8p8q63vDU2AO3A+Q6bm67cG6XGq6Zqs1N.Pf0rl0zMN4ebpB.pMrgM3ZAKXAIjSN4jQN4jyDxImbNa.bM.3hBtb4oTpp60mKNRdq.3AsjslPS2.3Q.vCqTpJ0cv3TX6uoHIaD5cZE+upTpqL3+20C9fOXhWxkbIolWd4kUVYk0X.vDAvoCfyA.W..lfYV30Vasn1ZqEyYNywLWshOw6UQEUrpoLkoDRSbPjLtJpnhEL4IO4WCRWIQnYABD3GWQEU7.CTVMnwFa7Kkc1Y+7VcbEhNtRoFutChvAIGE.pW2wgYnwFabUiZTi5kzcbLXH46.iuW0wqrxJK6YLiYLrm3BKnfBbcwW7EOpLxHil6uYgZRlC.tcnutZU6s2d62yt28te1Eu3EeTXT4bQXvVWw7fcU.6vA0xAvolrys.ABD.20ccW3RtjKAe5O8mVGgPrpt6niN9xokVZaAgviRO3XC3GCf6BRWVQnYd858I1wN1wsc9m+42xfsbtc6dFiabi6PVUbEl9XkRc15NHBGj72hXjYT28su8sf4Lm4rccGGCFRlL.99.3N.PZZNbFNZWoToakEXgEVX7yYNy4LF23F2U.i7s+3hxE4583wyuN4jStbmxb9gckcuh4wA.aY+eyJ4wiGroMsIbC2vMbJybohHxGbvCdvKdVyZVCZkZ5MR9qfwXDPHzom4i9nO5eegKbg0EJKbCMzPV4jSNMEsCpHzqoTpKQ2AQnhjiA.gz3OwInzRK8KeZm1o8B5NNBEjTc3Ce3jTJUNokVZSZLiYLmOLterctOUeBc2c2OmKWtVsNiARl..xDF8.grgQxIf.noCe3C2T6s2da6cu606d1ydh6ltoaZlie7ieWgvp8ifwOb5CbhYWI6JacEye228cyXoKcogbkmh042ueDe7Nl4DJ6peI.tiv4WzSxyBFSnSBgt7h0Vase6wMtwU8PunehfiEB65rq4SpTpuotChPEI+8.3qq63vrzc2cWiKWtlHrGOU5HRmc1Y9ImbxuG.xS2wxfokVZ4GjUVYYm562CpxKu7jyO+7GnIXQev3oGuIkRcbKLrFwvV+H4at4lWjtiA6DoR4COABDXsJk5GElUJWAf+RTLrDhAyKCfIqTpuX3Vob.f4N24ZmG7bGS2APnhjiCwPUJG.Ht3haBjzVWg1gRJojxgO1wN1Yo63XnzXiMd.cGCgpBKrv3yO+7ew94sdV.LeXLof8KkJkG8XaqoGIys5pq9k0cbHhY76hO93ezH3ycFPSiu.wHZuH.tYkRUwvYknTptIY2vd1HL0n6.HL3XZsyvv6s4Mu4v9G6Y2LwINw55niN9cojRJWutikARqs15v55XqRvAN56gOY1W8OBfGC.aWoT10m7VLG63Mq6oUJ+a4laton6XQDSn0ZpolaIB+r+XSMRDhA2yBizf1Wb3Vo7doJSZ8X1Bo9IutEr0x+Z5NNLatc69KaWyg4gq8u+8+eq6XXvzZqsZ6OWO3LP9c.faqgFZXJadyaNdkR80TJ0GHUJ2ZYWaw7ICizOnPLbUJ.VXt4la6g6GL3fkQqCXGwHF+u.3pUJUzn6cbLXbOU6lPJEkZCDK1Z4u4DlvDbq6fvrzUWcUptigAS6s2tseR0IXCA33mMaiEXKawb.by5N.DwDdH.b5JkJRyJE4ahwhPzu76++e6cuGeTVcs2.+2JISlbkbgPBPf.ALnHAQJ3EzhZphWnspUKwVsd9T6QqZa8zSa0Z0dQssGeaea01i5odrpG8T0d7jz5EN7pBpPDEE0DEABfbK2SfDBSBISlIyjYVu+wL3IDRHSl444YuelY88yG+HjLyduRlGRVy9YsW6gVIQTElTR4..5Z4Jn8mdlwqqV9.CLvco5XvHwLq0MIBWtbMVajRg33ncqXdUUUUx.3Go53PXq0I.t.hncFiiyYZDAiPLVZqs1V7LlwLL6N9iVdf3zbyMGyG1JVfGT0AfYHiLx3STcLXjV1xVlV2p9prxJ04MgsPyncqX9ke4W9bgFFWBai+YDpFci0jxA.NeCXLDhwzN1wNFDleaqUKqu0csqco02del4hPni.83MOIQT7Vhh5bKebPn2wmPyncI.6zoyqV0wfvV5k.P9DQ+qDQF0FZ50Av2bqacqyb+6e+yxfFSg3yrhUrhcvLGjY1Ey7qwLufva9cijVVxHd73Q2u89+AUG.lA+98+npNFLAi6o2rBsCUG.B6EcqTVH.b6pNHD1J6D.WsAsB4GChnpG1eMYl04e1uvF6kZrwF+AkVZoMAyIACsr9aSM0T8o5XXrDd0x+5pNNLCM1XiaW0wfQiHh03e97tUc.HrWzpDy22912j.P9pNND1BGF.WEQzFgErZI+g+veHaydNDIbFzmOemgSmN2lIOOZYh4qbkqTmqK33xUKG.qYdyadwqs9Ncse8q0cLFg9QqtHdNyYNye7eThDbtAvWA.ERD8VvhtElKdwK9aYEyiHgR4VPR4..8YAyQzPKq6Vl4og3zUKG.2upC.Sjt9FNZU0AfvdQqVwb.rRUG.BskG.7s.veyp23RO+y+7kedm24EO1KiEpyyRDsWKZtzwDy04SBwn4DB1tnVUG.lHccCspqsqTglR2RL+qo5.PncbCf+A.rZqNg7G6wdLGW5kdo2XIkTxexJmWQ7uibji7KsvoaBe3ZYA1kpCfQCy7L.vUo53vj7vDQ5b4CEqz0u1zx1UpPeoMIlyLmL.JS0wgPazCBkP9qXfcYkHFy7B.Pc.voUO2h3eqcsqc+V3zoic+DcstaeLUG.ln+npC.SlVVZTPS6JRB8kNUi4Yo5.PnEZ..U.fBHh9eTQR4gkE.1.zzd.svV6uUYkUZkWWqiIl2npCfQZfAFXVH9sbJ6Dg9Yqwyz0Dy0xMesPeoMIl6wimbUcLHTpM.fERDMWhnZTXB4..fH58IhtLhnoPgj7u427axeKaYKUDHPf0nxXSXuM3fC9ea0SoEOeQh1Tc.LRomd5OkpiASzOfHRa6mfFDcMw7ATc.HrWzlDyegW3ElgpiAgRb+82e+EQD8EHh1Nz2CJhf20ccWtV7hWbMojRJe4CbfCjE.tAnmI8HzXG3.GnNKdJ0tqQGZng5T0wvv40q2xPn6TW7pUq5.vBnqIl6U0AfvdQaRLuhJp3yq5XPDcd9m+4uZ.jL.RdvAG7JhfmR8.3Rpqt5RkH5mlc1YqU+R5Hwzl1zbSD8zHTIubcPe23QBMSCMzfU2kFztCxGe97cXUGCCC4zoymS0AgIpZhn9UcPXAzt2.ZXxuaPLgXzG8yQMl4FAfbrmqHtc6FYlYlviGOXKaYKHqrxByadyCNcN968Qhnjw+6pUPLyCgi+M80N.t21au8Wr3hKNtqtsYlSC.+N.78TcrHzZCBfzsxxJfYNS.nUIl0UWcMuBKrv8n53..nu95agYkUVaU0wgI5TMiSFYcCy7t.vIq53XTjhpKMSg8hVzUV5omdxGRR4J2C8PODFbvAwO9G+iA.v0ccWGdzG8QQ1YeBOzKaBG6sPjAv2D.eU.7Q.XiM2byaojRJom34ZbjHxK.tsAGbvGK0TS8cAfbRgJFMqVA+6.sq+N61saco2pSYkUVVcM+akNDQjV1ZJMA5ZIinqkXiPSoEkxRN4jyMp5XHQWlYlIV0pVExKu7vUe0WMJpnhvy8bOG1wN1w38TOt1KFQzyPDcEDQ2GQzFl0rlkq34jxGNmNct8VZokhPhQMcJl3dcELmZ2p0ELXPsnSwzau8dl.Hd9Dm96C8ce6XzzwtehmDke2mv3n7RYY6ae6otfEr.cs1vDiuBHhjCPgQAy7jAvkfP2AgKDG6aDNX2c2cI986umoN0otW.LUEDhBKle+9OiTSMUK8zWjYNInYImWSM0jdEUTgRWgyveeoQ.LSUFGlrLIhRH5JHLyuH.txd6s2+obxImGR0wSX0SDUtpCBg8hxWw7ErfErTUGChn1uPRJerQD0MQzekH5hIhRo5pq1YM0TS1g6nKNJnfBZaZSaZtesW609hpNV0TuTe8027qu95cBfjB21Jo8u+8aaaspc2c2MqfoUGWwNkWdMc2c2WDhuSJ++HQIo7vZA.ORN4jyin5.YXZQ0Afv9Q4qXNy7CBfefpiCwD1l.v4QDI0OWrKElYYm6++x+QNxQt5bxIm+mQ6SVas0lwRVxRzwiY9wUc0UWpKcoK0xeslYN.zfEh4nHhRBp8MLjLy7g.fs8M4EAJkHpQUGDVEl4qrwFa70JszR8xL2Lzi2z0SRDIkpqXBQG17mWtpC.wD1fszRKqnjRJQRJ2.DNooDMAAviejibj+Re802m1SO8zWlYlYR4jSNNyKu75OmbxYL+dRZoklxWs0nTepHo7vZC5QhJ.gZebJcU7OvANv0f36jx2GQTSpNHrRDQuzQ+yACFryjRJIc35cqt0nJhCnCqfxbUc.HlXpu95u4RJoDsXyaEO3gdnGJUUGCVnOr0VacY0TSMNIhtkbxIm2cFyXFcWd4k6qzRK0a94meuiWqEaG6XG102HiJOwXOfBm6QZ+pbxqpppRsnhJ5oUYLXA9NPOKgIqhtzdPi6ZMvBymRWw7MrgMnCqXuXBpnhJpdUGCwSV9xW9on5XvrELXvWYm6bm2Z4kWdLWi0UVYkAY1VlywFT3bePEN2iz9T4jWQEU7OC.GpLFLYAqqt5T40ZJWRIkjtrvQRh4hILklX7hW7hkdWtMTAETvivLeYDQ8fD6UkwPTVYkcKpNFLQ02RKsbkkTRI60.GS650behBmacJwbksg3dhm3IxtfBJ32pp42h7yUXISoKzkdZtNcB2JrITVorvLS4jSNGWOvVXKbV.3vLyAYlalY9JTc.YW8XO1i4HqrxJtLwbe97c8.3zL3jxOJcpzLhTprle6Tgy8Hor5tcUqZU+AUM2Vkd6s2+cUGCZ.c4Mlni8VcglSk0X9BPnd6rvdal.3k74y2RXlkRSZB5htnK5rTcLLN1Vas0147xu7KOom7IexIsu8suudD7b5bfAFXFNc57YMwt1Salz3ZlbopINPf.ZSaM0qWuJ4MUs4Mu4YLoIMo+QUL2Vn0lat4JqRKfOUG.goK05tvFQkIR8iT3bKLXNb3nV.fv09aO.Xa.3sAPsd85ca0We8sujkrD4TPaDlyblyuW0wvXIPf.2PJojx+IN1RG44c4xkubyM2+9X7zVS80W+UWd4ka1+hw1.vRL44vH4lHRYIK31s6COoIMIUM8GC2tc2kBlV5LNiynJELuVsaW0AflPWVwbaYacUnVJYEyYlSFgNMDEwmxE.KG.2M.dgzRKs8DtuSeXl4GfY97b4xUtPC5i9pzt10tJ.gJKHsie+9WZJojxSiQodte8W+0eY.LxSqW+.3ZHhtbKHob.8ploiDprirf96u+dT47Obd73wxuyAG7fG7bSJojVlUOuVrNIhjMleH5RmaRW1DpBaDUUJKknn4UnV4BfeH.dqbyMWWLy8xL+uvLuHlYmpN3rZm7IexeWUGCiFe97UdpolZci0muxJqL..90.nj8u+8OK.bpHzQ+cUv51Xl1sDyeSUN4tb4pWUN+CmWudszXY6ae6oVXgE9JV4bpH2LruaLZiltbVGnKaBUgMhpJkkKRQyqPujMBsp52M..y76CfeC.1H.bEOW1K6d261I.tWUGGije+9OKmNcFIq51ukHRk2tXsoloiPaQkSNybepb9GNe97Yowx7m+7e.D5m0DWq95qOQ3MeDozke2gtTq6BaDUsh4Z4JEJTtyB.uHBkzU6Ly2Jy7zQbXIuTVYkckpNFFogFZnKI0TS8ChjGqhSJGv9kXtpOEF0lMglOe9Fvplq95quElTRI88rp4Sg9UVTIjIlXzkUtWXiX4IlWas05..KxpmWgsyTAveB.sEtjW9gLySmY11mjNybR.P2ZoYeSGNbrtH4AxLqCmVuZSMSGgTVGYA.HojRxxRFd73ymOK416WUUUkbVYk0aXEykp8oe5m9PpNFzLZwumn95qWWp0cgMhkmX9RVxRJ1pmSgsW1.3APnNwwAXluQl4BTbLEKNaDpd60E+bhn+yH4Atu8suh.vEaxwSjPapY5HvgT8cXHPf.ZSstNv.CLxMNroXUqZU2J.JzJlKEasmxobJxIL4vzSO8jtpiA.fcricHIlKlvTQornkcgBgsQg.3wAPWLy6kYtRlYaS8iFdE+eZUGGCyyBf+kH8AOm4LmBIhzgU62Ncvc7hpN.BDHfkjLbDxzu89Ly4.fG1rmGcPe802OT0wfto0VaU4Il61saTYkUZVmiCh3XpHw7JUvbJhOMW.7eCfivLudl4kYCNjilO.JS0AQXeH.9lSjMYKQz1fdrwpzlMy33IPf.qWChAcp9ish5t8Qsf4PGz9jlzj1opCBcSJojhxWrFud8BnG+rRgMiUmXNA.kto2dm24ctiMtwMdt6e+6eEd734OoxXQXnp..uK.FjY9ewsa2Eqo0i9eV0APX8.fKfHxVdqVc61ss4f6n1ZqcqpNFRKszzlMgVEUTgotJhLyk.fH4DpMdvMAI4uiSVYkUNpNFRN4jsM+LJgdwRSL2kKW4X0y4Hl+WX4Ke4+9y+7O+2ctyctuQFYjw288du26VTU7HLEIAf6NiLxnU.7oLyWLyrCUGT..LyyD.mqpiivVLQj1rg.mn74ymtcvcTeGczwrIhRlHJo.ABbMG8S79u+62pJCL.sqVWMsDIC+lw+ql03qYBBfWW0AgNJqrxR46AorxJqCn5XPXOYoqnHy7R.PsV4bNbc1Ym2cQEUz+mg+wB2gLtdDptk0hD3DFtf.3WAf+MhHUbbfC..l4mG.Wy39.MeeChnmS0AQrH7ARktrgFOP80W+rFd6paDwWxDQptVSIlYUGC..dHhxvrFbl4EB.keGJrH2AQzuW0AgNZngF5HImbxptbVdOhnyQwwfvFxpW85OuEOeGiLyLyTG4GiHJX3NRQF.3KB02ugEFuj.v8.fNYleCl4E.q+MklOzfjxaokVdahn3gUTT08Q8Oy92+9+GFkdH8Q+6ukFjTNPnUoVGtKCsahiMAfpMwwWq3xkqGW0wftRCRJGv9cVKHzDVch4JMwjLyLyw71aQDMDQzq.fRAvbPndnsH9yEBfsyL2.ybELyIaQy6OwhlmSnlat4uFhCpI0vI6pEcZjppppi6t.NrMT6e2hCmSjFUc..fNMqAlY9yAfS1rFeMy+U94mucpkgZYBeWv0AGV0AfvdxJu.NI.rLKb9FMyX7d.DQLQTCDQeW.3D.WD.9XSOxDVsYAf0C.WLyekpppJSKA81ZqsL.vcXVi+DPOm64dtcn5fv.EQmRols1au8ST8a+NVVfL9zg6FnYsJhD.dFSZr0Q2spC.cUM0TitjXtROTwD1WV1EvM2by5vApxzlHOXhHeDQuIQzmC.4iP6.d4fbH9R1.3EV0pVUutb45pfI7uIl9zm92xnGynzOXhzZDsAdWUG...21scaSYL9TCBf8Zkwx3PGRL2TVEwCe3CWNB0JRSDTOziWK0Ryd1yVWZYt1oyZAgFwxRLepScp5vw38wUi4QJhHWDQOABc.2LKDZyDpM04pHlkYt4l6emYts1au8y.FTMnWas05..OnQLVwnWC.+EUGDFr2S0A..PYkU1LGiOUi.neKLTFO5vcKwLJ+BJu7xKQZ0x+1wYuAaCUVYkkVzDGBDHfjXtHpXYIl6vgiy1plqSfVh0AHbotzLQzu..oCfEBfmLliLgtXpSaZS6CXlWKybLeWdVzhVzWBpua+r5pqt5ujlrIDMRaW0A.Bsp3exX74tSMKAJcn8sY3snSWtbsH.rHidb0T8AfMq5fPmUPAE3T0w..Pu81qN8lxE1HVYsX8Urv4Zr7rF4fQDEfHZ6DQ2HBsZ7KC.qwHmCgxrBDp9yu9X3fJJoTRIkmvHCpnvGVc0UeUUVYk5Ter1nzrBm6N862+YTc0UmIQzXUKoq1Rinw2AUc...C8PWgYNobyM2+lQNlZtaIN7MXazRW0A..PvfAkCXHQTwpRLmPnSlQU5FfI1gDHh7SDsYhnubiM1X5HzlFcSl07IrL+E.71LySZh9DGXfANaDZuInJGB.mWbZR4fHxO.9CpXt6ryNWTpolZsmnu2pYqVNfdzkHLzjU7506J.fNTljVEcpK+nqzgVkHRJojzkyYAgMikjX9N24NyxJlmwxZW6ZOKhnm1pVogRKsTug2zne9N6ryrGZngtbL12tag96bQnUOe4SfmCkd5o+eXVATjvsa2KjHJt9WNTe80+SfBZahEVXglVa+yDo7tDQf.ALrDyerG6wbjVZokHkn5ulHRKZQnZt7Tc...je94qCma.BaHKIw7hKt3wsMEZlV8pW8NT0bWTQE0uCGN9eHhN88u+8maf.ApD.6QUwiHpkD.1Hy7uDQv+toqt5ZdPg8T4gFZnJRDNRnKu7x8cvCdvepUOutc6tPqdNM.JuuWGHP.CKFtga3Ftc.joQMd5tibji7GUcLXSLVcIIql7lnDQEKIw7ryN6SyJlmwRYkUlVz9jl6bmauojRJUSDMu5qu9I6wimqGRauxt4myLu9cu6ceB2fQETPAOpUEPih6ygCG0nv42Rsl0rFKuFiyLyLeAK7voxnn7ZdkY1P5TE6ZW6p.GNbb+FwXYS7R4jSNxIIYjoXUG...tb4RRL2hvLSLyIwL6fYNMl4LYlygYNel4BYlmJy7zYlKN7etPWtbkayM2b5ZzAR0mwpBnKzhlmQ0sdq25zU47OZJu7xObFYjwyRDUZM0TyT5qu9tQnGcMAw367Kqrx9Tl4QsVFas0VmIT2dp38.vuTQysR31saU7uaVF.doXXiAqBJurlBFLnQrh4zIexmbhzF9Dd8581UcLXiLKUG...6XG6PZmxQIl4jYlSObh0kvLuvvmT2eMl4amY9gYleYl4sxLO..BBf..vG.7fPso1dPnCzrChPsJ11.Pqg+yGL2by00Lm4LG..ABON2Iy77YlU9B4ZI+REl4tgZ2DbeQhnWQgyejhdu268Jb9ye9ekbxImeE.JP0Aj3D5v.XNDQGSxFLyuJ.tTEDOd5ryNKrnhJJQqMcQLyppSULahHawc8hY1AB8KtTl96u+SK6rydawxX3wimKLszR6MLpXxFXmDQK..51lIVKwL+x.3xUcbzRKsblkTRIenpiCcSUUUUxqZUqJc.jC.JB.yD.kBfSE.md3+S0sX3+D.9c.nIUrI9M8Dyqs1ZcrjkrDk8KCb4x0rxKu7NBQTOpJFhRza8Vu0TW3BW3UmWd4cePsuwFwXqcDJ47AA.1+92+IWZoktKUDH9746zc5zYB4lLlY1K.TQ+K96PDoxxVJhEd08UZq1yiGOyJiLxHpaykM2byoOyYNSWPMuVqD986+bSM0T0hS4V6.l4FgdrDd+vPB..f.PRDEDUp4KgH5iTcPnBgKOjLQnDumC.NMDpIJbA.PGNE3iT6C.eC.7AVYaJ0zSL+.G3.EUTQEorRzXaaaaS8zNsSSG5euwB5ce22c5yadyaUSdxS9df85B6DA+6DQ2J.RhYd2PMsuseBQzuUAyqVfYd6.XAJXpWMQzUnf4MpvL6BJ7mezau8let4laT2cXXleV.bcFXHo65C.4J8t7HiN7lOGlSmHJdegRHl4il.97Av4.fqL7eNdRKHzcg4SrhUP2zSLmY9rfZOoxlGQT7TWPgdi23Ml9RVxRtlbyM26EZROaUfj8506uJszR6tUvb+g.3rSj+k2LyOJ.tEEL0dHhxPAyaTgYtF.b9JLDRKZa4e9746y6vgi21nCHM2WmH54UcPXWDt9f0hZ61mOeKxoSmaU0wgAgps1ZSeAKXAElVZoMO.bV.3KF9+mHYs.nRhHCYSrOVrhDyuUDpdcTg0.fqHNNgE5C9fOXFkWd4Uld5oeOPRRWkN..lpBl2fc2c24UPAEXp+fBcGy7UBfWTQSeRZ3gIznhY9g.vsovPHp9dEybVHzAlUBSIr..rm8rmzl27lmzcOhPc0UWYqK+rPWtbsn7yOeaUh4LyIUe80mQ1YmcgEVXgkkVZocl.3xPnM6tHjf.3RIhdcyZBrhce5EaAywX4ZiiSJG.fOyy7LaA.O..dv8rm8LiRKszqI4jS9W.KHI8.ABfZpoFzbyMioLkofhKtXL6YOajWdZw46fUSEIkC.bg5xuHRwjSY2HyNU3b+BQYR4DBsHKITIkCfetjT9DiGOdlvmPylEhHsqM7cTUUUUIWRIkj4LlwLlZd4k2ImQFYb1HTB3KdAKPEUDnsRR.XcLy+Vhn6Blvlx1JVwbUsor..bPDMjhlaUhFXfAlQ5om9WG.+LXxIo6xkKricrCrm8rG7ge3Gh96uebdm24gktzkhEtvEhjRRa+4S1cOFQjJJeCsS3j21Ir9C0I+DQoZwyYTyiGOWbZok1ZUwb60q2xRO8z26D84o365pxTWc0k6RW5RU9gBkcRO8zyRxImbpU0wA.vgNzgNioLkonzXgYNIWtbkclYl4LRM0TW..97HTsRqCaN13AOL.99F8cL0TSLmYNUn1S+pjiyWw7HwQSR+Z.fkrR5ACFD8zSOfYFSdxS1rmtDUGpt5pa5KcoKUKpmRcPe80W4YkUVwTq3KJ7dDQmiEOmQsFZngSY1yd1JYUyewW7EK3ptpqZBcH4vLOK.zn4DQZsmiH5an5fvtwsa2e0LxHipUcb..zWe8s7IMoI8NV07s8su8TWvBVvT.vofPIfe0.XgV07m.aEDQFZ6a0rKkEUVSCusjTN..3LxHiV.vuG.O.y7L.foljdRIkDxOeo6NZlZu81WtjT9wJ6rytdl45g01cV9KV3bEyZs0VO3rm8rUxb+du26MgZatg2HeIZa1S..r28t26R0wfcTFYjwhTcLbTolZplV9O6d261YYkU1TQn1P3ECfuFjy8DUY0LySxHqNCytFCJwjG+wzfCN3uSUysFiIhZgH52SDkCB85ysiPsjKKSiM1380Ymc9qCFL3Gakya7hfAC9aKt3hURuRWywd734lsv4qS.7LV37EyV9xWtx1OBACFbhtPI+TD5vGIQyGVVYk0hpCBapUn5.3nb5zogz1bqpppRt+96epLyqfY9AXl6prxJyKBcmjVM.9dPRJWkRGgZQjFFyNw7SyjG+wTu81qbhachczjzefvIoOS.7iPniwVSSWc0Et1q8Ze4hJpneQxIm7miHJkMtwMVRmc142B.0alycbhd93O9i+4pNHzUqYMqYyvZJetRHhJhHxsELWFo..PIsO1rxJqHtNLYlmM.tWSKXzX8zSOeaUGC1TDzq122kDMOocu6c6zqWuyiY9lXl27pV0pFJyLyrC.rN.7CgjDtN5wCenJYHL6DyUVsW1UWccXUM21PLQTqDQOHQT9.XF.3G.SHI8oLkofMtwM9QG7fGro1au8atolZZVACFrihJpnmhHp7ZpolrapoltvfACpjMnltyiGOeQoDVFaUVYkA.fUzK46vBlCyx+iJlz4N24FQ6oIl4jAv5M4vQW0dd4kW79gRioXm6bmYo5XXDtzppppS3FCmYlN3AOXVLymNy7syLu2xJqLuNc57SAveF50azPL1xE.eNUGDQDl4NX0wz63LI.Hl4hYluMl4tM4WudzAFXfiozmtm64dRq4la97YlWuIO21EC.KnSJY2sicriIa1uP31s6kn5uNiVLyWqY+8mQyi7HORDk3Dy7Mqh3SG32ueU1dgs074y2RU8qeiTc0U2y9TO0SkK.Rt1Zq0QWc0U1LymDy7UxL+mYl6WsQnv.sQi5ZYS6WxygV0CU0pB2CQz7TzbGuhXlmJBsSuuGXR2NsN6ryWpnhJ5ZAfmg+wW6ZWalKZQK5BKpnh9+Bquk3oKtZhnWP0Agc.ybs.vLSdty5qu9YVd4kOg1Pi5.e97cFNb33Cr548Ue0WMmUtxUdBqwcl4bfIWNcZrAqt5pyL7c8QLAvgVHt2B.KW0whHwUu81a94latth0wwLKkEUdLUK2JPiGSD0AQziPDUH.lF.9NHzFfyvTXgEdkCMzP8tksrkuFFVWC5RtjKw8Tm5TWMQzort0sth5omd9AXDIuGGqk1au8YIIkG45u+9ucSdJJbAKXAqqpppJYSddLb80WeGPEya1Ymcj78pG2zCD802URJOpMSHIkKTrbxImulQLNlYh4prAV2lBm6DALQzAHhdTD5Dubp.3VPnik9XVxImriEsnE8ewLejVas0u48bO2SZC+yeIWxkzYd4k2ejHJqMu4MuHud89RFw7poNPc0U2oTbwE2rpCD6jlZpo20BllyeUqZU++XCbS+XERJojrztvzQMzPCcBSLmCsgOWk0DMZomS0AfMVkpN.DB.7agATIJl4uPQYsJQXvqhqXrQDwDQGjH5w.vzAPQ.3FAP6FvvmdwEW7Scu2685YvAG7eq1ZqsDbrWzGbYKaYaM8zS+q7xu7KOo1ZqsuEhutM3A+zO8SW3RW5RGP0AhcS3RL4wrfo5RPnMJssQt4lqRtSSYmc1NFmGxSXIAhd5WSD4U0AgM1OR0AfP.fr83wSLeppZlIlqxZ7VY8p2DYgSRuShnmDg5rKSA.2..h4dxapol52YIKYIMwL+os2d6m+HKgfq7Jux9lwLlwSQDM42+8e+SevAG7Mi04T05qu9N+S4TNkCo53vtp6t69e0hlpeOybZi+CSanj89yjm7jcNVet1ZqsR.vEZggit4AUc.XW0byMmNBcWaEBkKszR6eLVGCyLw7Eahi83QUa5TQXgSR+PDQOM.lEBkj92Bw9JoW1zl1zpYUqZUtb4x0WE.i71iG7rO6y9SRKsztnG4QdjBZs0VuuXb9TkGyJONmiGUPAE7ovZ5o4..mgEMOwLUchHO4IO4zGqO2zm9z+yVYrnY9uHhh4MLVhpIMoIMMUGCBwvbmwZ4MZlIlqx9uorAZzHCKI8mB.yn+96uH.7sQrUxQYmat4VMyrqN5niu.Fk5551tsaq6YNyYduOzC8PoUWc0cMCMzP1kxbom5qu9+IUGDwABBfeoEMWWpEMOFAFV2aX4yjc1YOpMDf1au8khn7vXINwcp5.vNKmbxIQsKcIzSN.vbhkAHtbEy6u+9OgM0eg5PDwYmc1cRD83DQS0sa2Gs6tDskrQ1ScpS8M84yWcu4a9lmLFkDz+9e+u+fKcoKsJGNbL40u90e9d73YewxWClMe97cA1w1vmNps1Z6YrnoxtkX4Gof4LmQ9A5omdxaZSaZFV++0F58Hhh4R8KA2xTc.HDivWOVdxlUh4Iahi8354dtmKOUM2hIDNqrx5.DQOJQTgc2c2yD.+DDEsAQGNbr3uvW3KrKud8to0st0MeL56L5fW3EdgaLiLxnr0st0s3AGbv5i0u.LAOrSmNk18oAYFyXFsB.q3T.VkktWz3uof4L+g+WXlWbN4jygAvXVhKI.tYUG.wAtRUG.BwHbmHF5NKlRxyu669tJcEqO0S8TKUkyuHpvETPAsRD8aAPV81aukgPaHpIT8v5zoykshUrhc3ymuOYyadyKAi9+3fujK4R1RZok1B27l27hBFL3mZ.wuQ3..3Gp5fHNCCfepELOIYy5o4ulBlyhFwe+mofXPmzD.1tpCBatj.vBUcPHDiPlgOPFiJlRh4olZpJMw7ku7kuBUN+hXCQTvbyM28RD8ipolZb1d6suDLAWgOGNbrvy5rNqZYlac+6e+WLN9MIJ..urksrslbxIO+1au8OOh9xownrLhHYiKavZu81sjUGtiN5Hkw+QoM1Er91J6LGwe+Br34W27MIhXUGD1Y8zSOGW4QIDZhntKSYJIlO0oNUU+KnlY3i1YgMWEUTwPEWbweDQzpt268dSukVZ4RCFL3DoTOldokV5ZYlc2c2ceSqd0qdz1.ZbwEW7lBWy62nQE6SP2BQTiJZtiqUbwEeH.rSKXpFy1AntIbmY4ygI3cjJFM+gGBXDk1RBl9.vaq5fvtKiLxnLUGCBwX3eNZehlRh4EWbwV9N9eX9jibjibJ.nXEFCBSv8ce2m2RJoj0lbxIe5UWc0St0Va8VPjefB4L+7y+O+k+xeY2CN3fO3V25VG4sUG.HPVYk0S9pu5qly.CLvZMvPe77ZDQIxsKNqvsa1Svoe5mtsZw.HhZafAF3brvo7LO5engFZv17lXLI2HQjz8vhQNb3XkpNFDhwvRXl0qFQhWudeWVAZt4lk5MKwB8BuvKL2N6ryGNJtb4cZqs19bXzeCpzZVyZpr2d60vuFcDNBauNbZrkZngFRynegyue+6hYNMl41Xl48rm8Xa5NDLy+Nl4uIy7YZzee4DAg+2Z9746rsx4UyDfYd7NETEiCl4j3P+7SgPWM6n4ZaSqyoLv.CnjM0R0UW8dUw7JTF9ptpqZeEVXg2V0UWsy8su8cgACF7iivm64N8oO85XlOR+82+M85u9qO7U7j+ReouTU4jSNetFarQSHrCwue+UHGE2luRKsTu.3wMxwLkTRoyvu1c4..Se5S+LGmmhNYV.3o.v6akS5gNzglFyrSGNb77V47pY9QDQ9UcPXmwLSABD3qCfrUcrHDm.mtpCfiAGZEYrT986mQLzhZDwO1vF1PA97461Yl8NQtFZngF5c15V25Jti63NxFguVZfAFXylzkr0B45UKCy7bL3W+ZN73lDyrKl4sp5uFiTLyWuA+8BQDpiN5HSU+5ucTs0VqCl4alYdWp90PgHB8qT8+t4XvLe+V82AZu816R0ecKzKUUUUI2SO8rTl4ZlnWO0byM6acqaccXBWpxLyra2tU4oiaBGlYhYd+F4qgH7arhY9GxLyu669t1hdxMybYF42GDQrGT0u1aCQ9746bYl6R0u3IDSD986+eKZtfWYGBPlggFZHs9DcTX8prxJCjat4VKQzEzRKsLYDZS.FQ2F4YNyY5XEqXEQcuHc7jQFYrEyZrEGuvsltawHGyitIFOxQNxZ..Niy3LNeib7MKG3.GncUGCIh5qu9teUGC1HI4ymuymYtAGNb7N.n.UGPBwDQJojRuQyyyLSL2x2bKojRJcX0yov9njRJ4vDQO..x.g5gxVQKzar3mHRkcunDUqG.F122m8rmc9..0We8s..jRJo7exLq8GzPSaZSa.UGCIfdoIMoIo5yJAs2t28tcxLeMLy83vgiZPn8CgPXG0Rz7jLyDyGs9EsoJiLxHRacdhDXDQCQD8V.XAd85cN.nZEDFefBlyDdgO.m9tF3PNG.fksrkczj8KD.WqAN9lEF.5xIdaBAud8Jmpum.s1ZqSlY9dJqrx7BfmGxF6TX+MQNyU9LlYh4SxDG6QUlYlorJPhHFQDmd5o2.QTk81auSF.+NKZpa..WuEMWhi2yZfi04.7YGXOGsDo9Kr83.NaipN.Rf7IokVZMp5fP2vLmTe802BYlesvGDX2qpiIgv.EocHtigYlXdtl3XOpRIkTjRCPDUxM2bOLQzOdW6ZWSB.OhYLGtc6t5ZpolzIhlCQTClwbHFegKgn6vfFtgu53CeOC7eXPiuoIPf.aS0wPhB+98eSg2iCB.zbyMmNy7WC.ckUVYsU.bIpNlDBC1kQD4IZdhlYh4p31PImjZhXx7m+76iH515niNNUCdn22q7JuxWuhJpP5Y45gG0fFmEwLmR3+7OYXe7qhYdNFzbXJb61srY4sFG1gCG0o5fP0Xlot6t6YxL+nyblyb..7eAf7UcbIDFrWA.yhH50h1AHtpTVPn5lTHhYuy67N61fGxUUYkUJuwQMAQjabrIRG0ZpollR3wb8aZSaZ3+bu+ciX7MKtc69fpNFRPbSgK0oDRbn97+YCfON+7yuYXvcFIgPgFDgRD+63ymuSC.oSD8EIhZNVFTS6vMgCcXarPyZ7GC+dhHi5VTKRfEd0NMpUTrIhnRg7FG0JLyNAPeHF6fTtc69KlUVY8JCabWO.pH7es.hntikw2rzTSMMmRJoDSYUy6ryNwTlxT.Qx4mU80WuyxKubepNNrZLyIgPknxyBYkwE1SGB.0hPahycgP6Or1C+wcSDYJ+65TF+GRTSEqPPbUeYWnTWkQMPCMzP2Hjjx0NDQCxL+OfP2R8nVlYl4+.BspIG0MAf8F9O+UAviEKiuYYvAGrOidL84yGdhm3IPu81Ktq65tL5g2N59RPSJ+T.vaBfoq5XQHFEAAP8.ntCe3Cu8t5pq84ymuVSN4j6xmOe85vgiAJu7x8CE86sMyDy62DG6wh12+fE1BD.9oFzX0hCGNVuAMVBiW0.3gQrc3kbMHzl.MH..Qz9XleR.7OBfeC.dbnlEp3DxmOeQ0FS5DYm6bm3u9W+qXMqYMF8PaK0d6saJajbcFy7kgi8MpJDFBWtbcA4kWdmEB08dFqSYY2.Xqg+usiPKRRaHzpb2G.7Bf.57lw1LKkkWF.WtYM9ig+DQjQ1ihEIfXlmJ.LhCqJ+.XtDQQ0gLfvZvLuT.7gwxXTWc0M8ktzk9YWyDtLYN..x8PG5PW3TlxTzt2bVs0VqikrjkXnqlamc1I5s2dQYkUlQNr1UafH5Kn5fvJE9v0ZHUGGB6k8t289idsW609nJqrR+EVXgYAfogPmQDmBBcWW7BfmgH5+DHzFIFgVH1TZrwFwN24N4UtxUF.gZ.HZaB2QJyLw7GF.eOyZ7GCONQz21hmSQbFl4aAwdW63v.XFQa6RRXsXlqFgJ6jnR+82+0mc1YeL8Gcud8dpNc5rd.zC.JxrpGwnU3Z.V1PxlD+98uzTSM0DptwBybpv.OYcEw8BBfqgH5uo5.QmXl0jsJZEWNUvbJhiD9cheew533xkqJjjxsUtADCkaRVYk0cOxOVZok1N.vuGgNSG9SQenYNFwghjvf8RuzKskw+QEeI7a9bSpNNDZMOHzYExYBfzjjxOdlYh4Fc6lKRLV0bjPDoJBgNV0iZACF70xO+72pAEOBK.QT+HTshGsl+G+we7wcnp0XiM9yQnD9+GYl0wNSwNTc.DOJPf.2aBb6Q0puS4B8U8.3eE.qB.mJBc91jIQzsQD8gDQxBCLJLyM+4dG+GhgSVwbQr5qDqCvN24NuYiHPDVKhn+Ny7qCfUDMO+EtvEdk.3oG9GqzRK06PCMzOJ4jS9O.fqCg1no5jO..KR0AQbluSJojhV2C6MYeBBkP1BTcfHLU6Dg1aNaC.6G.s60q2NGbvA6oyN6z87l277g3f58VELyZLOKDZGvZkdchnK1hmSQbhvkwRmH15PGuEQzEXLQjvpwLmABs+.hl2jeeUWc04MxUJcG6XGSd9ye9GBg5J.EpScC.l4aC.Ojpii3DM0e+8elYmc1cp5.Q0XlKGgRXSXe8w.XiG4HGYKd85cud85s8Ce3Ce3O3C9.227MeyCAIoaSiYlXNgP+Bti616ZhdahnyyBmOQbDl4BAPLcZHN3fCdxokVZpnLtDFjXoKs3ymuqwoSmUMhOLwLGD.3PG5PSZJSYJV8BVLl7506k4zoSo01Y.5t6tyofBJ3HpNNzAg+8+uO.NCUGKhwja.rA.7NCMzPaq2d6sg8su80YSM0TeUVYkJqGdKLYLy2Mas9.U+0rv9hY9aDqW+wg9ERBaNl4eSTdMPfZqs1LFkwacLyra2tWhJ95Yrru8suEFiWyKB4kU8qk5lAGbvEn5WTDrWNzO64tYluLud8dxMzPC4xLalkwrHFYpIQvLeR.XOl4bLBeBQzoagymHNBy7N.v7iggXtDQ62nhGg5vgZkf0.fkGEO8miH5aLhw6zPnZu8WQD8Kh8HzX79u+6O0y7LOSinm8mnKehHWpNHzLDy7APLtY5EQjsAf0Bf22mOeepWudaukVZouDwSc13Al5QXe0UWcCl43OJR0hmOQbBNzdhHVRJ++VRJO9Q3VI3EiP0E9D0041s6ydDersAfWG.+XXx+b2IhAFXfdTcLDG3RkjxGUbyM278p5fHNhe.r1AGbv63PG5PWXiM1Xoexm7IYQDkDQzoQDcGDQ+MmNctsbxImtkjxsuL8a6te+9e1TRIkqyrmmv1CQz7rn4RDGgY97QnUHMZDD.4Dtk6IhivQ+o.qmVZokIWRIk7Y8xdl4rAP2CLv.mWlYl4lMrfL17Y0+tXhKXvf2exIm7OU0wgt50e8W+LtnK5hjRLchIne+9eyCcnCs1t5pqOr4lade0Vascee228MHj59VXD5niNpvBqmJUzhFEwAXleln8hN+98eIpN9ElGl4yHJuz3EwHV7Cl4OGybG0VasNTzWNGGNzdiPLA4ymu+ebnRdRLFZngFtRU+5jl6fLyOTu816kussss4t5Uu5LfErfohDbadyadRV3E4Rh4hILl4jigq49WTc7KLeLye4n4hC+98eb8Eel4JYl23F1vFzhMfEy7uLFt9OgTf.A1JL2yADaOlYhYtCU+Zkl4cXluVWtbM6W4UdE4bWQnLDyrKK5hdIwbwDFybIQ40aalkULKgAy7MEMWjzWe8UznLV+Pl4pgFr5XLyWZTd8ehp1ppppjSY5wAGJw7co5WrTrCxLemG4HGY9ae6aW1CbB8Ay78aQ+ifco5uVE1OLy2PzbwVO8zSdpN1EVKl4edTboRiLyGWoqvLeqLyWtJ95XDwwrilq+SP4ZyadySR0ulYWvL6jY9uq5WzTf6gYVtNQDUrpU6a0Vz7LjEMOh3Ke+n34bq4latRmXHwyuF.+elfOmYAfmhGQOtmH5QAvgXlS1nBtnzAT77aW3ot5pqry9rOa4PDJBQDMH.9p.3zAfU2k1TF+98+ZDQx0IhnhUkX9t..1xV1xO..IOv.CLCDp0+Xz7ZBioHN1t28tcBfEMAeZ8AfmvDBGgliHhAvOE.+tI3S85B+eib7dWhn.FQrEsHh7hPmBfhwl+lZpo4tzktzno8YlPiHhIh9D.bR986+7QBvaD7fGLlN.oEI3rjDyIh5E.A6omddDhnfYlYlsAf+ISXpjDyESHkTRIyYh9bFZngtVhH4tyjfJbx42I.9USvm5yvLWlIDRFgmQ0AfFy+gO7gm6rm8rkChoX.QTvTSM0MBfh83wy4CflTcLYVFZngT9dGQXeYUqXNCfeWEUTwvSl4oAvfF77zmAOdh3bNc57hlfOEOu3K9hupoDLBaivImeO.3GLAepeLybllPHESN7gOrbjxO576xkqSZxSdxsn5.IdAQTvLxHiMRDUJ.VJ.pW0wjPnSrxNJwCM7+R3ae5JM34nWCd7Dw+9tSvG+MWYkUpzROPnGBeK5+i.3Jl.OsLAvaxZV27olZp4iTcLngFbfAFXN4me9Mq5.INESDUG.VH.JC.ujhiGgPKXY+xAhn1GkO15AviYfSS2F3XIhyEt8UcxSjmyd1ydpxjBGgMEQzpAvIgH+N1cV.39LuHZh6ptpq5Pvb12O1Ud.vryLyLaU0AR7tvuA28RD8U.Pd.3aiDf5PWHFKJeUapt5p+t.XmFzvIIlKhXEWbwSeB9T9CyadyynK+JQb.hn8Afo.fmKBeJ+Ll4ywDCoIDhnf.3wUcbnI5C.yhHRRNzhQD0CQziSDMsVZokL.vZUcLIDVMkmXdkUVYfZpolyDFS8gKaEZQDK2bycwSjG+92+9+ilUrHr+HhFjH5a.fSEQ1hMTiN0qi84y2Sp5XPCbHDJo7tTcfjnqjRJwy68du2WA.AUcrLQ4vgCYyeJhZJOwb.fJpnh92yd1yoZ.C0wUtLBwIvDYie1zbm6bkM.lXbQDsS.r..Le.T8I3g5..uFzfS+S.fTSM0sp5XPwZB.kRDImOAZhy4bNGOd734dTcbHDVIsHwb.f4Mu40J.N2XbXZyHhEQBiKC.6IPf.WO.1zI5A1e+8+SQntKjPLtBW2r6hHpRhnz22912EBfWeTdnKiY9Zr53azDtEfNQaAjwKpC.mBQT+pNPDGq5pqt+hpiAgvJoMIlCD5v1Xe6ae2YLLDRelULQbS.3TRIkTd1N6ryK8D8.2zl1jzhDEQKumzIcRqmH5hqu95chPk5x2C.uBBsJsmlRiti0D8fSJdvKAfyNbmBSnY5niNjEbSHTIl4jXl+.N53T0wuvdpmd5IuSv0UcCMobCDByFy7YGk+7W6n6m0rVWo33MzPC8Fp9BkIhFarQsYicKreztefDQTP2tcOQ5Kv6zue+W..tXhHoiYHhJ4jSNW7I3S+vPJiEQBBhnMO3fCd6pNNr.qhH5tC2QZDZLWtb82TcLHDVEsKwb.frxJqN.vGNhObvAFXfq..WC.9WC+eWHQT4olZpuEQznU+lBwnhYddi3Ck+X8X862uzxtDITV+5W+in5XvDcX.bRDQRxd1DczQG0p5XPHrJon5.3D3rAvLNzgNzTJnfB5G.MkYlYdzZ.TNjWDwpEAfcOr+9XVGi974qAyObDB8wJW4JGjYdM.3Ko5Xwf83.31j6tp8RGczQyKbgKT0we6TFa..fApjDQAQkgPXIz1DyCe6EaN7+IDlpCdvCt8hJpnQ8y40q2Ar3vQHzA2IhORLOH.9M.3gkCMH6o.ABzqpigIHYOIIhZZYorHDVfOY3+kMtwM1D.7zPCMbE.X4.XaG8ykUVYUrEGaBgxQDsC.bGpNNhRd.v8iPc.mTIh9oRR41WqbkqzmpiAgPHDVLl4zF1elXluLl4ABuQ6i0druPXKwLe8po2VLwDHPf+du816Wtu95qPlYYEKiyvL+Qp9ZrHUSM0zxU82uD1WZaorHDVsg2GiIhX.7pbniL8YVc0UKkTkHgDQzyvLWC.1..lqhCmixM.dV.7h82e+aI6rytKXCO51ESHaB.KV0AQjfHRpFAQTSRLWHNABeZHJa9SQBMhnV.PYLymG.dRXsIn2I.dA.rd.rE.zJ.7F9MOKRbrcUG.Qp.ABHIlKhZRh4BgPHhDLQzawLWF.lN.p.gZesKE.S0.F+2F.qC.ezfCNXCd858.e7G+w8UQEULjAL1B6u8o5.HRkTRIkrpiAgPHDBQBJlYppppJ4Zqs1bN3AO34xLW6InDb6lY9g8506UzTSMM2Zqs1LfzEKDiCud8dple0gaLZngFtTU+8Kg8krh4BgPHhIgKqj..nW.rIl4yD.eAD57nX..rqd6s2O0kKWsUZok58DLTBwnpmd5o6wpk1paHhjbqDQM4hGgPHDFpvmCEuQ3+SHhYABDnOUGCQpjRJIGpNFD1WxFTPHDBgPn01zl1jc5zZ0opC.g8kjXtPHDBgPqUYkUF.gN3nzdIkTRoM9OJgXzIIlKDBgPHrCdGUG.QBIwbQrPRLWHDBgPXGrYUG.QhjRJoLTcLHrujDyEBgPHD1AaQ0APjPRLWDKjDyEBgPHD1A6V0APjH0TSMKUGCB6KIwbgPHDBgcP6pN.hDolZpYq5XPXeIIlKDBgPHrCNhpCfHgCGNxS0wfv9RRLWHDBgPn8HhFB.sn53X7jRJoHIlKhZRh4BgPHDB6hmW0APDXxpN.D1WRh4BgPHDB6hWS0APDnPUG.B6KR0AfPHDBgPDIXlK..co53Xb3mHJUUGDB6IIwbgPHDBgs.yLA.O.vopikSDhnj..q53PX+HkxhPHDBgvVfHhAvCn53X7D9MPHDSXxENBgPHDBaCl44Bf8p53XbjJQjeUGDB6GYEyEBgPHD1I6G.6S0Aw3HEUG.B6IIwbgPHDBgsQ3xYYUpNNFGZcMvKzWRh4BgPHDB6ls.fUq5f3DHCUG.B6IIwbgPHDBgsR3UM+qA.2pNVFC4n5.PXOIIlKDBgPHrcHh7.fyR0wwXXJpN.D1SRh4BgPHDBaIhn5AvMn53XTTjpC.g8jjXtPHDBgv1hH5oAv8o53XDlppC.g8jjXtPHDBgvViH59.vCq53XXllpC.g8jjXtPHDBgvtiIh99.39UcfDVgpN.D1SRh4BgPHDh3AL.9Y.31Tcf.oqrHhRRh4BgPHDh3BDQLQzi.fyF.AUXnvJbtE1XRh4BgPHDh3JDQuOB0xB2lhBgCnn4UXyIIlKDBgPHh6PDcX.b5PMsSwMof4THDBgPHDB8FybFLy+Xl4.r4qqFZngzT8WyBgPHDBgPnsXlS1iGOylY95Xl+HSHo7sxLmgp+5THDBgPHDBak29se675pqttYlYuFPR4uLybJp9qIgPHDBgPHrs1vF1PJG9vG9xYl6OZxHOPf.OHyrru8DBgPHDBgvHTas05fY9mMAyK+FYlIUG6BgPHDBgPD2gYdxLyO03jP9tYlKS0wpH9h7N7DBgPHDhQAybl.3L.vWD.KD.d.vlAvKQDsaHGjPBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPHDBgPLb++0mbtckFOhda.....jTQNQjqBAlf" ],
									"embed" : 1,
									"forceaspect" : 1,
									"id" : "obj-2",
									"maxclass" : "fpic",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "jit_matrix" ],
									"patching_rect" : [ 38.5, 18.584905660377359, 100.0, 52.830188679245282 ],
									"pic" : "rave.png"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-69",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 647.0, 275.0, 73.0, 22.0 ],
									"text" : "loadmess 1."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-65",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 696.0, 412.0, 29.5, 22.0 ],
									"text" : "+~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-66",
									"maxclass" : "live.dial",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "float" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 724.0, 309.0, 45.0, 48.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "Bias z1[3]",
											"parameter_mmax" : 3.0,
											"parameter_mmin" : -3.0,
											"parameter_modmode" : 3,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "Bias",
											"parameter_type" : 0,
											"parameter_unitstyle" : 1
										}

									}
,
									"varname" : "Guidance[1]"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-67",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 696.0, 382.0, 29.5, 22.0 ],
									"text" : "*~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-68",
									"maxclass" : "live.dial",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "float" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 689.0, 309.0, 45.0, 48.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "Bias z1[4]",
											"parameter_mmax" : 3.0,
											"parameter_mmin" : -3.0,
											"parameter_modmode" : 3,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "Scale",
											"parameter_type" : 0,
											"parameter_unitstyle" : 1
										}

									}
,
									"varname" : "Guidance[3]"
								}

							}
, 							{
								"box" : 								{
									"bubble" : 1,
									"bubbleside" : 2,
									"id" : "obj-64",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 784.0, 220.0, 141.0, 66.0 ],
									"text" : "Or even entirely generate our own latent positions from scratch."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-63",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 609.0, 412.0, 29.5, 22.0 ],
									"text" : "+~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-61",
									"maxclass" : "live.dial",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "float" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 636.0, 309.0, 45.0, 48.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "Bias z1[2]",
											"parameter_mmax" : 3.0,
											"parameter_mmin" : -3.0,
											"parameter_modmode" : 3,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "Bias",
											"parameter_type" : 0,
											"parameter_unitstyle" : 1
										}

									}
,
									"varname" : "Guidance[2]"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-54",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 460.0, 549.0, 91.0, 22.0 ],
									"text" : "s~ audio.output"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-55",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 461.0, 255.0, 82.0, 22.0 ],
									"text" : "r~ audio.input"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-56",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 442.0, 119.0, 70.0, 22.0 ],
									"text" : "loadmess 0"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-57",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 442.0, 224.0, 61.0, 22.0 ],
									"text" : "enable $1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-58",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 442.0, 148.0, 26.0, 26.0 ],
									"svg" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-59",
									"maxclass" : "newobj",
									"numinlets" : 8,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 461.0, 481.0, 106.0, 22.0 ],
									"text" : "nn~ wheel decode"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-60",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 8,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 461.0, 292.0, 106.0, 22.0 ],
									"text" : "nn~ wheel encode"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-53",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 474.0, 176.0, 306.40000456571579, 33.0 ],
									"text" : "The advantage of this modularity is that we can directly work in the latent space rather than the audio space."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-52",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 271.0, 177.0, 159.200002372264862, 33.0 ],
									"text" : "Expose the latent variables (here identical to forward)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-51",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 62.0, 177.0, 163.0, 33.0 ],
									"text" : "Go through the whole model (encode then decode)"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Ableton Sans Bold",
									"fontsize" : 16.0,
									"id" : "obj-50",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 474.0, 148.0, 144.0, 26.0 ],
									"text" : "Advanced usage"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-46",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 256.0, 404.0, 91.0, 22.0 ],
									"text" : "s~ audio.output"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-47",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 256.0, 256.0, 82.0, 22.0 ],
									"text" : "r~ audio.input"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-44",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 55.0, 396.0, 91.0, 22.0 ],
									"text" : "s~ audio.output"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-43",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 55.0, 256.0, 82.0, 22.0 ],
									"text" : "r~ audio.input"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-42",
									"lastchannelcount" : 0,
									"maxclass" : "live.gain~",
									"numinlets" : 2,
									"numoutlets" : 5,
									"outlettype" : [ "signal", "signal", "", "float", "list" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 55.0, 488.0, 43.0, 83.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "live.gain~[4]",
											"parameter_mmax" : 6.0,
											"parameter_mmin" : -70.0,
											"parameter_modmode" : 3,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "live.gain~[4]",
											"parameter_type" : 0,
											"parameter_unitstyle" : 4
										}

									}
,
									"varname" : "live.gain~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-41",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 55.0, 436.0, 89.0, 22.0 ],
									"text" : "r~ audio.output"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-38",
									"maxclass" : "ezdac~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 55.0, 582.0, 45.0, 45.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-35",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 701.0, 110.5, 84.0, 22.0 ],
									"text" : "s~ audio.input"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-26",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 238.0, 119.0, 70.0, 22.0 ],
									"text" : "loadmess 0"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-29",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 237.0, 225.0, 61.0, 22.0 ],
									"text" : "enable $1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-30",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 237.0, 149.0, 26.0, 26.0 ],
									"svg" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Ableton Sans Bold",
									"fontsize" : 16.0,
									"id" : "obj-31",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 271.0, 149.0, 144.0, 26.0 ],
									"text" : "Encode / decode"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-12",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 29.0, 119.0, 70.0, 22.0 ],
									"text" : "loadmess 0"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-36",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 28.0, 226.0, 61.0, 22.0 ],
									"text" : "enable $1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-39",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 28.0, 149.0, 26.0, 26.0 ],
									"svg" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Ableton Sans Bold",
									"fontsize" : 16.0,
									"id" : "obj-17",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 62.0, 149.0, 99.0, 26.0 ],
									"text" : "Forward"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Ableton Sans Bold",
									"fontsize" : 24.0,
									"id" : "obj-28",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 184.0, 310.5, 41.0, 35.0 ],
									"text" : "=="
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-27",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 55.0, 317.0, 107.0, 22.0 ],
									"text" : "nn~ wheel forward"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 609.0, 382.0, 29.5, 22.0 ],
									"text" : "*~"
								}

							}
, 							{
								"box" : 								{
									"bubble" : 1,
									"bubbleside" : 2,
									"id" : "obj-19",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 613.0, 220.0, 141.0, 66.0 ],
									"text" : "We can mess up the scale and bias of each dimension separately."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-98",
									"maxclass" : "live.dial",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "float" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 602.0, 309.0, 45.0, 48.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "Bias z1[1]",
											"parameter_mmax" : 3.0,
											"parameter_mmin" : -3.0,
											"parameter_modmode" : 3,
											"parameter_osc_name" : "<default>",
											"parameter_shortname" : "Scale",
											"parameter_type" : 0,
											"parameter_unitstyle" : 1
										}

									}
,
									"varname" : "Guidance"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 8,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 256.0, 341.0, 106.0, 22.0 ],
									"text" : "nn~ wheel decode"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 8,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 256.0, 293.0, 106.0, 22.0 ],
									"text" : "nn~ wheel encode"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-234",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 195.0, 86.5, 452.0, 33.0 ],
									"text" : "The model decomposes different functions so you can impact parts of the process.\nNote that the number of latents varies depending on the model."
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Ableton Sans Bold",
									"fontsize" : 18.0,
									"id" : "obj-226",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 29.0, 89.0, 158.0, 28.0 ],
									"text" : "RAVE - Functions"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 18.0,
									"id" : "obj-37",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 173.0, 14.0, 455.0, 27.0 ],
									"text" : "RAVE - Realtime Audio Variational auto-Encoder"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 48.0,
									"id" : "obj-40",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 14.0, 14.0, 149.0, 62.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-18",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 173.0, 38.0, 482.0, 33.0 ],
									"text" : "This model works by encoding audio to a (compressed) latent space, from which we can then decode back to audio in a target domain in real time (performing timbre transfer)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 906.0, 465.0, 41.0, 22.0 ],
									"text" : "sig~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-15",
									"maxclass" : "newobj",
									"numinlets" : 6,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 906.0, 436.0, 107.0, 22.0 ],
									"text" : "scale 0. 127. -3. 3."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 6,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 778.0, 436.0, 107.0, 22.0 ],
									"text" : "scale 0. 127. -3. 3."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"maxclass" : "pictslider",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 778.0, 297.0, 147.0, 125.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 778.0, 465.0, 41.0, 22.0 ],
									"text" : "sig~"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-35", 0 ],
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"midpoints" : [ 787.5, 430.276458442211151, 787.5, 430.276458442211151 ],
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"midpoints" : [ 915.5, 425.276458442211151, 915.5, 425.276458442211151 ],
									"source" : [ "obj-11", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-39", 0 ],
									"hidden" : 1,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"midpoints" : [ 787.5, 460.276458442211151, 787.5, 460.276458442211151 ],
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-16", 0 ],
									"midpoints" : [ 915.5, 467.570601969957352, 915.5, 467.570601969957352 ],
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-59", 3 ],
									"midpoints" : [ 915.5, 496.800008177757263, 770.528575594936115, 496.800008177757263, 770.528575594936115, 471.600008428096771, 507.785714285714278, 471.600008428096771 ],
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-63", 0 ],
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-23", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-30", 0 ],
									"hidden" : 1,
									"source" : [ "obj-26", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-33", 0 ],
									"source" : [ "obj-27", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"midpoints" : [ 246.5, 328.300004601478577, 265.5, 328.300004601478577 ],
									"order" : 0,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"midpoints" : [ 246.5, 289.300004601478577, 265.5, 289.300004601478577 ],
									"order" : 1,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-29", 0 ],
									"order" : 1,
									"source" : [ "obj-30", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-34", 1 ],
									"hidden" : 1,
									"order" : 0,
									"source" : [ "obj-30", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-44", 0 ],
									"source" : [ "obj-33", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-46", 0 ],
									"source" : [ "obj-34", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 0 ],
									"midpoints" : [ 37.5, 304.300004601478577, 64.5, 304.300004601478577 ],
									"source" : [ "obj-36", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-33", 1 ],
									"hidden" : 1,
									"order" : 0,
									"source" : [ "obj-39", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-36", 0 ],
									"order" : 1,
									"source" : [ "obj-39", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"hidden" : 1,
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-42", 1 ],
									"order" : 0,
									"source" : [ "obj-41", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-42", 0 ],
									"order" : 1,
									"source" : [ "obj-41", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-38", 1 ],
									"source" : [ "obj-42", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-38", 0 ],
									"source" : [ "obj-42", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 0 ],
									"source" : [ "obj-43", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-54", 0 ],
									"source" : [ "obj-45", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"source" : [ "obj-47", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-34", 0 ],
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-60", 0 ],
									"source" : [ "obj-55", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-58", 0 ],
									"hidden" : 1,
									"source" : [ "obj-56", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-59", 0 ],
									"midpoints" : [ 451.5, 327.100001573562622, 470.5, 327.100001573562622 ],
									"order" : 0,
									"source" : [ "obj-57", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-60", 0 ],
									"midpoints" : [ 451.5, 288.100001573562622, 470.5, 288.100001573562622 ],
									"order" : 1,
									"source" : [ "obj-57", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 1 ],
									"hidden" : 1,
									"order" : 0,
									"source" : [ "obj-58", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-57", 0 ],
									"order" : 1,
									"source" : [ "obj-58", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-59", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-59", 2 ],
									"midpoints" : [ 787.5, 496.800008177757263, 693.385717498404574, 496.800008177757263, 693.385717498404574, 471.600008428096771, 495.35714285714289, 471.600008428096771 ],
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 0 ],
									"midpoints" : [ 470.5, 376.046877288026735, 618.5, 376.046877288026735 ],
									"source" : [ "obj-60", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-67", 0 ],
									"midpoints" : [ 482.928571428571445, 364.575002117082477, 705.5, 364.575002117082477 ],
									"source" : [ "obj-60", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-63", 1 ],
									"source" : [ "obj-61", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-59", 0 ],
									"midpoints" : [ 618.5, 446.281257737893611, 470.5, 446.281257737893611 ],
									"source" : [ "obj-63", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-59", 1 ],
									"midpoints" : [ 705.5, 457.800007909536362, 482.928571428571445, 457.800007909536362 ],
									"source" : [ "obj-65", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-65", 1 ],
									"source" : [ "obj-66", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-65", 0 ],
									"source" : [ "obj-67", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-67", 1 ],
									"source" : [ "obj-68", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-68", 0 ],
									"hidden" : 1,
									"order" : 0,
									"source" : [ "obj-69", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-98", 0 ],
									"hidden" : 1,
									"order" : 1,
									"source" : [ "obj-69", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 7 ],
									"source" : [ "obj-7", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 6 ],
									"source" : [ "obj-7", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 5 ],
									"source" : [ "obj-7", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 4 ],
									"source" : [ "obj-7", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 3 ],
									"midpoints" : [ 302.785714285714278, 329.300004601478577, 302.785714285714278, 329.300004601478577 ],
									"source" : [ "obj-7", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 2 ],
									"midpoints" : [ 290.35714285714289, 329.300004601478577, 290.35714285714289, 329.300004601478577 ],
									"source" : [ "obj-7", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 1 ],
									"midpoints" : [ 277.928571428571445, 329.300004601478577, 277.928571428571445, 329.300004601478577 ],
									"source" : [ "obj-7", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"midpoints" : [ 265.5, 329.300004601478577, 265.5, 329.300004601478577 ],
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"hidden" : 1,
									"source" : [ "obj-9", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 1 ],
									"source" : [ "obj-98", 0 ]
								}

							}
 ],
						"originid" : "pat-313"
					}
,
					"patching_rect" : [ 78.0, 306.0, 42.0, 22.0 ],
					"saved_object_attributes" : 					{
						"globalpatchername" : ""
					}
,
					"text" : "p rave"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"order" : 0,
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"order" : 1,
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"order" : 2,
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"source" : [ "obj-8", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"source" : [ "obj-9", 0 ]
				}

			}
 ],
		"originid" : "pat-309",
		"parameters" : 		{
			"obj-2::obj-42" : [ "live.gain~[4]", "live.gain~[4]", 0 ],
			"obj-2::obj-61" : [ "Bias z1[2]", "Bias", 0 ],
			"obj-2::obj-66" : [ "Bias z1[3]", "Bias", 0 ],
			"obj-2::obj-68" : [ "Bias z1[4]", "Scale", 0 ],
			"obj-2::obj-98" : [ "Bias z1[1]", "Scale", 0 ],
			"obj-3::obj-23" : [ "Bias z2[1]", "Bias z3", 0 ],
			"obj-3::obj-4" : [ "live.gain~", "live.gain~", 0 ],
			"obj-3::obj-95" : [ "Bias z2", "Bias z2", 0 ],
			"obj-3::obj-98" : [ "Bias z1", "Bias z1", 0 ],
			"parameterbanks" : 			{
				"0" : 				{
					"index" : 0,
					"name" : "",
					"parameters" : [ "-", "-", "-", "-", "-", "-", "-", "-" ]
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "cherokee.aif",
				"bootpath" : "C74:/media/msp",
				"type" : "AIFF",
				"implicit" : 1
			}
, 			{
				"name" : "jongly.aif",
				"bootpath" : "C74:/media/msp",
				"type" : "AIFF",
				"implicit" : 1
			}
, 			{
				"name" : "nn~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "rave.png",
				"bootpath" : "~/Documents/Max 9/Packages/nn_tilde/misc",
				"patcherrelativepath" : "../misc",
				"type" : "PNG",
				"implicit" : 1
			}
 ],
		"autosave" : 0
	}

}
